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

Game::Game(): m_player(m_clock, 160, 160), m_pauseText("Paused", resources().font(), 50)
{
	m_pauseText.setPosition(400, 450);
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
	
	m_keyHandler.onKeyComboPressed({ sf::Keyboard::S, sf::Keyboard::LControl }, [this]
		{
			m_map.save(getSaveFile(m_map.getFile()));
		});
	
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::Escape, [this] { stateMachine().pushState<MainMenu>(); });
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::Q,      [this] { stateMachine().reset<MainMenu>(); });
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::Z,      [this] { stateMachine().pushChild<GamePopupMenu>(); });
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::R,      [this] { stateMachine().switchState<Game>(); });
	m_keyHandler.onKeyPressed(sf::Keyboard::Key::P,      [this] { toggle(); });

	m_keyHandler.onKeyPressed(sf::Keyboard::Key::M,				[this] { m_music.toggle(); } );
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::Add,		[this] { m_music.incVolume(); });
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::Subtract,	[this] { m_music.decVolume(); });
	
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::W, [this] { m_renderSprite.move( 0,  7); });
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::A, [this] { m_renderSprite.move( 7,  0); });
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::S, [this] { m_renderSprite.move( 0, -7); });
	m_keyHandler.whileKeyPressed(sf::Keyboard::Key::D, [this] { m_renderSprite.move(-7,  0); });

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
		auto &createdItem = m_itemFactory.create(item->type);
		createdItem->construct(item->gid, item->name, item->sprite);
		createdItem->applyProperties(*item);
		m_player.takeItem(std::move(createdItem));

		layer->removeSprite(item);
	});

	m_intersectionHandler.registerEvent("Entrance", [this](ObjectLayer *layer, ObjectSprite *entrance)
	{
		std::filesystem::path mapFile;
		if (!entrance->getProperty("EntranceTo", &mapFile))
			return; // Give user a message, invalid entrance

		auto pos = static_cast<sf::Vector2i>(m_player.getPosition());
		entrance->getProperty("SpawnX", &pos.x);
		entrance->getProperty("SpawnY", &pos.y);

		if (!loadMap(m_map.getPath() / mapFile))
			return; // Give user error message, invalid entrance
		
		m_player.setPosition(pos.x, pos.y);
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
	m_renderTexture.clear(sf::Color::Black);
	m_fringeDrawer.sortObjects();
	m_map.drawWithFringe(m_renderTexture, "Fringe", m_fringeDrawer);
	m_renderTexture.display();
	
	// Draw to target
	target.draw(m_renderSprite);
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

	m_renderTexture.create(m_map.width * m_map.tileSize.x, m_map.height * m_map.tileSize.y);
	m_renderSprite.setTexture(m_renderTexture.getTexture());
	updateDrawRect();

	return true;
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

void Game::updateDrawRect()
{
	auto playerpos = m_player.getPosition();
	auto playerbounds = m_player.getLocalBounds();
	auto windowsize = window().getSize();
	auto newX = windowsize.x / 2 - playerbounds.width  / 2 - playerpos.x;
	auto newY = windowsize.y / 2 - playerbounds.height / 2 - playerpos.y;
	m_renderSprite.setPosition(newX, newY);
}
