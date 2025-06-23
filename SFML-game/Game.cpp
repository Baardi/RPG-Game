#include "stdafx.h"
#include "ObjectSprite.hpp"
#include "ObjectLayer.hpp"
#include "MainMenu.hpp"
#include "GamePopupMenu.hpp"
#include <filesystem>
#include "Weapon.hpp"
#include "Valuable.hpp"
#include "Enemy.hpp"
#include "App/StateMachine.hpp"
#include "App/ResourceManager.hpp"
#include "SavFile.hpp"
#include "Game.hpp"

using appstate::Game;

Game::Game() : 
	m_player{ m_clock, { 160, 160 } },
	m_pauseText{ resources().font(), "Paused", 50 }
{
	m_pauseText.setPosition({ 400, 450 });
}

Game::~Game()
{
}

void Game::init()
{
	State::init();

	m_itemFactory.registerType<Weapon>("Weapon");
	m_itemFactory.registerType<Valuable>("Valuable");
	m_spriteFactory.registerType<Enemy>("Enemy");

	m_fringeDrawer.addObject(m_player);
	
	m_keyHandler.onKeyComboPressed({ sf::Keyboard::Key::S, sf::Keyboard::Key::LControl }, [this]
		{
			m_map.save(getSaveFile(m_map.getFile()));
		});
	
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::Escape, [this] { stateMachine().pushState<MainMenu>(); });
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::Q,      [this] { stateMachine().reset<MainMenu>(); });
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::Z,      [this] { stateMachine().pushChild<GamePopupMenu>(); });
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::R,      [this] { stateMachine().switchState<Game>(); });
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::P,      [this] { toggle(); });

	m_keyHandler.onKeyPressed(sf::Keyboard::Key::M,						[this] { m_music.toggle(); } );
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::Add,			[this] { m_music.incVolume(); });
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::Subtract,	[this] { m_music.decVolume(); });
	
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::W, [this] { if (m_renderSprite) m_renderSprite->move({ 0,  7 }); });
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::A, [this] { if (m_renderSprite) m_renderSprite->move({ 7,  0 }); });
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::S, [this] { if (m_renderSprite) m_renderSprite->move({ 0, -7 }); });
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::D, [this] { if (m_renderSprite) m_renderSprite->move({ -7, 0 }); });

	m_intersectionHandler.registerEvent("Enemies", [this](ObjectLayer *layer, ObjectSprite *sprite)
	{
		auto enemy = dynamic_cast<Enemy *>(sprite);
		if (!enemy)
			return;

		enemy->fight(m_player);
		m_player.fight(*enemy);

		// Probably need a better way, such as a foreach method on map
		if (enemy->isDead())
			layer->removeSprite(enemy);

		// Find out what to do with player
		if (m_player.isDead())
			stateMachine().switchState<Game>();
	});

	m_intersectionHandler.registerEvent("Items", [this](ObjectLayer *layer, ObjectSprite *item)
	{
		auto createdItem = m_itemFactory.create(item->class_);
		createdItem->construct(item->gid, item->name, item->sprite);
		createdItem->applyProperties(*item);
		m_player.takeItem(std::move(createdItem));

		layer->removeSprite(item);
	});

	m_intersectionHandler.registerEvent("Entrance", [this]([[maybe_unused]] ObjectLayer* layer, ObjectSprite* entrance)
		{
			const auto mapFile = entrance->getProperty<std::filesystem::path>("EntranceTo");
			if (!mapFile)
				return; // Give user a message, invalid entrance

			const auto spawnX = entrance->getProperty<int>("SpawnX");
			const auto spawnY = entrance->getProperty<int>("SpawnY");

			if (!loadMap(m_map.getPath() / *mapFile))
				return; // Give user error message, invalid entrance

			if (spawnX.has_value() && spawnY.has_value())
				m_player.setPosition({ static_cast<float>(*spawnX), static_cast<float>(*spawnY) });
		
		updateDrawRect();
	});

	loadMap("data/Maps/Intro village.json");
}

bool Game::frame()
{
	if (!State::frame())
		return false;

	m_keyHandler.handleKeyInput();

	if (!m_paused)
	{
		m_player.handleKeyInput(*this, m_map);
		m_intersectionHandler.handleEvents(m_map, m_player);
		m_clock.reset(true);
	}

	return true;
}

void Game::pause()
{
	State::pause();
	m_music.pause();
	m_map.pause();
	m_clock.pause();
}

void Game::resume()
{
	State::resume();
	m_music.play();
	m_map.resume();
	m_clock.resume();
}

void Game::draw(sf::RenderTarget &target)
{
	// Draw to renderTexture
	if (m_renderSprite.has_value())
	{
		m_renderTexture.clear(sf::Color::Black);
		m_fringeDrawer.sortObjects();
		m_map.drawWithFringe(m_renderTexture, "Fringe", m_fringeDrawer);

		m_renderTexture.display();

		target.draw(*m_renderSprite);
	}

	// Draw to target
	m_player.drawToWindow(target);
	if (m_paused && stateMachine().isCurrent(this))
		target.draw(m_pauseText);
}

bool Game::loadMap(const std::filesystem::path &mapFile)
{
	auto mapToLoad = getLoadFile(mapFile);

	Map tmpMap;
	if (!tmpMap.load(mapToLoad, resources().textures(), m_spriteFactory))
		return false;

	if (m_map.getFile() != "")
		m_map.save(getSaveFile(m_map.getFile()));
	
	m_map = std::move(tmpMap);
	loadProperties(m_map);

	try
	{
		m_renderTexture = sf::RenderTexture({ m_map.width * m_map.tileSize.x, m_map.height * m_map.tileSize.y });
		m_renderSprite.emplace(m_renderTexture.getTexture());
	}
	catch (const std::exception &)
	{
		return false;
	}
	
	updateDrawRect();

	return true;
}

void Game::loadProperties(const MapProperties &properties)
{
	loadMusic(properties, m_music);
}

void Game::loadMusic(const MapProperties &properties, Music &music)
{
	if (const auto musicFile = properties.getProperty<std::filesystem::path>("Music"))
		music.load(m_map.getPath() / *musicFile);
	else
		music.reset();
}

void Game::updateDrawRect()
{
	const auto playerPos = m_player.getPosition();
	const auto playerSize = m_player.getLocalBounds().size;
	const auto windowSize = window().getSize();
	
	const float newX = windowSize.x / 2 - playerSize.x / 2 - playerPos.x;
	const float newY = windowSize.y / 2 - playerSize.y / 2 - playerPos.y;
	
	if (m_renderSprite)
		m_renderSprite->setPosition({ newX, newY });
}
