#include "stdafx.h"
#include "Game.h"
#include "ObjectSprite.h"
#include "State.h"
#include "MainMenu.h"
#include "GamePopupMenu.h"
#include <filesystem>

Game::Game(): m_player(m_clock, 400, 400), pauseText("Paused", font, 50)
{
	pauseText.setPosition(400, 450);
}

Game::~Game()
{
}

void Game::init()
{
	UI::init();
	m_keyMapper.AddActionKey(sf::Keyboard::Key::Escape, ControlKey::No,  State::Push<MainMenu>);
	m_keyMapper.AddActionKey(sf::Keyboard::Key::Q,      ControlKey::Yes, State::Reset<MainMenu>);
	m_keyMapper.AddActionKey(sf::Keyboard::Key::Z,      ControlKey::Yes, State::PushChild<GamePopupMenu>);
	m_keyMapper.AddActionKey(sf::Keyboard::Key::R,      ControlKey::Yes, State::Switch<Game>);
	m_keyMapper.AddActionKey(sf::Keyboard::Key::P,      ControlKey::Yes, std::bind(&UI::toggle, this));

	m_keyMapper.AddActionKey(sf::Keyboard::Key::M,		ControlKey::Yes, std::bind(&Music::toggle,    &m_music));
	m_keyMapper.AddActionKey(sf::Keyboard::Key::Add,		ControlKey::No,  std::bind(&Music::incVolume, &m_music));
	m_keyMapper.AddActionKey(sf::Keyboard::Key::Subtract, ControlKey::No,  std::bind(&Music::decVolume, &m_music));

	
	m_intersectionHandler.Register("Entrance", std::bind(&Game::HandleEntranceIntersections, this, std::placeholders::_1, std::placeholders::_2));
	m_intersectionHandler.Register("Items",    std::bind(&Game::HandleItemIntersections, this, std::placeholders::_1, std::placeholders::_2));
	
	m_map.load("data/Maps/Intro village.json", State::Textures());
	LoadProperties(m_map);
}

bool Game::frame()
{
	if (!UI::frame())
		return false;

	m_keyMapper.HandleKeyInput();

	if (!m_paused)
	{
		m_player.HandleKeyInput(m_map);
		m_intersectionHandler.HandleIntersections(m_map, m_player);
		m_clock.reset(true);
	}

	return true;
}

void Game::pause()
{
	UI::pause();
	m_music.pause();
	m_map.pause();
	m_clock.pause();
}

void Game::resume()
{
	UI::resume();
	m_music.play();
	m_map.resume();
	m_clock.resume();
}

void Game::draw()
{
	m_map.splitDraw(window, "Character", Map::DrawType::Back);
	m_player.draw(window);
	m_map.splitDraw(window, "Character", Map::DrawType::Front);

	if (m_paused && State::IsCurrent(this))
		window.draw(pauseText);
}

void Game::LoadProperties(const MapProperties &properties)
{
	LoadMusic(properties, m_music);
}

void Game::LoadMusic(const MapProperties &properties, Music &music)
{
	std::filesystem::path musicFile;
	if (properties.GetProperty("Music", &musicFile))
		music.load(m_map.GetPath() / musicFile);
	else
		music.reset();
}

void Game::HandleItemIntersections(ObjectLayer* layer, ObjectSprite* item)
{
	m_player.takeItem(item);
	layer->removeSprite(item);
}

void Game::HandleEntranceIntersections(ObjectLayer* layer, ObjectSprite* entrance)
{
	std::filesystem::path mapFile;
	if (!entrance->GetProperty("EntranceTo", &mapFile))
		return; // Give user a message, invalid entrance

	int x = m_player.x;
	entrance->GetProperty("SpawnX", &x);
	
	int y = m_player.y;
	entrance->GetProperty("SpawnY", &y);

	Map tmpMap;
	if (!tmpMap.load(m_map.GetPath() / mapFile, State::Textures()))
		return; // Give user a message, invalid entrance
	
	m_map = std::move(tmpMap);
	LoadProperties(m_map);
	m_player.SetPosition(x, y);
}
