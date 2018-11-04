#include "stdafx.h"
#include "Game.h"
#include "ObjectSprite.h"
#include "State.h"
#include "MainMenu.h"
#include "GamePopupMenu.h"
#include <filesystem>

Game::Game(): player(clock, 400, 400), pauseText("Paused", font, 50)
{
	pauseText.setPosition(400, 450);
}

Game::~Game()
{
}

void Game::init()
{
	UI::init();
	keyMapper.AddActionKey(sf::Keyboard::Key::Escape, ControlKey::No,  State::Push<MainMenu>);
	keyMapper.AddActionKey(sf::Keyboard::Key::Q,      ControlKey::Yes, State::Reset<MainMenu>);
	keyMapper.AddActionKey(sf::Keyboard::Key::Z,      ControlKey::Yes, State::PushChild<GamePopupMenu>);
	keyMapper.AddActionKey(sf::Keyboard::Key::R,      ControlKey::Yes, State::Switch<Game>);
	keyMapper.AddActionKey(sf::Keyboard::Key::P,      ControlKey::Yes, std::bind(&UI::toggle, this));

	keyMapper.AddActionKey(sf::Keyboard::Key::M,		ControlKey::Yes, std::bind(&Music::toggle,    &m_music));
	keyMapper.AddActionKey(sf::Keyboard::Key::Add,		ControlKey::No,  std::bind(&Music::incVolume, &m_music));
	keyMapper.AddActionKey(sf::Keyboard::Key::Subtract, ControlKey::No,  std::bind(&Music::decVolume, &m_music));

	
	intersectionHandler.Register("Entrance", std::bind(&Game::HandleEntranceIntersections, this, std::placeholders::_1, std::placeholders::_2));
	intersectionHandler.Register("Items",    std::bind(&Game::HandleItemIntersections, this, std::placeholders::_1, std::placeholders::_2));
	
	map.load("data/Maps/Intro village.json", State::Textures());
	LoadProperties(map);
}

bool Game::frame()
{
	if (!UI::frame())
		return false;

	keyMapper.HandleKeyInput();

	if (!paused)
	{
		player.HandleKeyInput(map);
		intersectionHandler.HandleIntersections(map, player);
		clock.reset(true);
	}

	return true;
}

void Game::pause()
{
	UI::pause();
	m_music.pause();
	map.pause();
	clock.pause();
}

void Game::resume()
{
	UI::resume();
	m_music.play();
	map.resume();
	clock.resume();
}

void Game::draw()
{
	map.splitDraw(window, "Character", Map::DrawType::Back);
	player.draw(window);
	map.splitDraw(window, "Character", Map::DrawType::Front);

	if (paused && State::IsCurrent(this))
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
		music.load(map.GetPath() / musicFile);
	else
		music.reset();
}

void Game::HandleItemIntersections(ObjectLayer* layer, ObjectSprite* item)
{
	player.TakeItem(item);
	layer->RemoveSprite(item);
}

void Game::HandleEntranceIntersections(ObjectLayer* layer, ObjectSprite* entrance)
{
	auto mapFile = entrance->GetProperty<std::filesystem::path>("EntranceTo");
	auto x = entrance->GetProperty<int>("SpawnX");
	auto y = entrance->GetProperty<int>("SpawnY");
	map.loadRelative(mapFile, State::Textures());
	LoadProperties(map);

	player.SetPosition(x, y);
}
