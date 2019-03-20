#include "stdafx.h"
#include "Game.h"
#include "ObjectSprite.h"
#include "State.h"
#include "MainMenu.h"
#include "GamePopupMenu.h"
#include <filesystem>

Game::Game(): m_player(m_clock, 400, 400), m_pauseText("Paused", font, 50)
{
	m_pauseText.setPosition(400, 450);
}

Game::~Game()
{
}

void Game::init()
{
	UI::init();
	
	m_keyMapper.addActionKey(sf::Keyboard::Key::Escape, ControlKey::No,  State::Push<MainMenu>);
	m_keyMapper.addActionKey(sf::Keyboard::Key::Q,      ControlKey::Yes, State::Reset<MainMenu>);
	m_keyMapper.addActionKey(sf::Keyboard::Key::Z,      ControlKey::Yes, State::PushChild<GamePopupMenu>);
	m_keyMapper.addActionKey(sf::Keyboard::Key::R,      ControlKey::Yes, State::Switch<Game>);
	m_keyMapper.addActionKey(sf::Keyboard::Key::P,      ControlKey::Yes, [this] { toggle(); });

	m_keyMapper.addActionKey(sf::Keyboard::Key::M,			ControlKey::Yes, [this] { m_music.toggle(); } );
	m_keyMapper.addActionKey(sf::Keyboard::Key::Add,		ControlKey::No,  [this] { m_music.incVolume(); });
	m_keyMapper.addActionKey(sf::Keyboard::Key::Subtract,	ControlKey::No,	 [this] { m_music.decVolume(); });
	
	m_intersectionHandler.registerEvent("Items", [this](ObjectLayer* layer, ObjectSprite* item)
	{
		m_player.takeItem(item);
		layer->removeSprite(item);
	});

	m_intersectionHandler.registerEvent("Entrance", [this](ObjectLayer* layer, ObjectSprite* entrance)
	{
		std::filesystem::path mapFile;
		if (!entrance->getProperty("EntranceTo", &mapFile))
			return; // Give user a message, invalid entrance

		auto pos = static_cast<sf::Vector2i>(m_player.getPosition());
		entrance->getProperty("SpawnX", &pos.x);
		entrance->getProperty("SpawnY", &pos.y);

		Map tmpMap;
		if (!tmpMap.load(m_map.getPath() / mapFile, State::Textures()))
			return; // Give user a message, invalid entrance

		m_map = std::move(tmpMap);
		loadProperties(m_map);
		m_player.setPosition(pos.x, pos.y);
	});

	m_map.load("data/Maps/Intro village.json", State::Textures());
	loadProperties(m_map);
}

bool Game::frame()
{
	if (!UI::frame())
		return false;

	m_keyMapper.handleKeyInput();

	if (!m_paused)
	{
		m_player.handleKeyInput(m_map);
		m_intersectionHandler.handleIntersections(m_map, m_player);
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
		window.draw(m_pauseText);
}

void Game::loadProperties(const MapProperties &properties)
{
	loadMusic(properties, m_music);
}

void Game::loadMusic(const MapProperties &properties, Music &music)
{
	std::filesystem::path musicFile;
	if (properties.getProperty("Music", &musicFile))
		music.load(m_map.getPath() / musicFile);
	else
		music.reset();
}
