#include "stdafx.h"
#include "MainGame.h"
#include "ObjectSprite.h"
#include "State.h"
#include "MainMenu.h"
#include "GamePopupMenu.h"
#include <filesystem>

MainGame::MainGame(): player(clock, 400, 400), pauseText("Paused", font, 50)
{
	pauseText.setPosition(400, 450);
}

MainGame::~MainGame()
{
}

void MainGame::init()
{
	Game::init();

	map.load("data/Maps/Intro village.json", State::Textures());
	LoadMusic();
	
	keyMapper.AddActionKey(sf::Keyboard::Escape, ControlKey::No, State::Push<MainMenu>);
	keyMapper.AddActionKey(sf::Keyboard::Key::Q, ControlKey::No, State::Reset<MainMenu>);
	keyMapper.AddActionKey(sf::Keyboard::Key::Z, ControlKey::No, State::PushChild<GamePopupMenu>);
	keyMapper.AddActionKey(sf::Keyboard::Key::P, ControlKey::Yes, std::bind(&Game::toggle, this));
	keyMapper.AddActionKey(sf::Keyboard::Key::R, ControlKey::Yes, [this]()
	{
		if (pausable)
			State::Switch<MainGame>();
	});
}

bool MainGame::frame()
{
	if (!Game::frame())
		return false;

	if (!paused)
	{
		gameTick();
		clock.reset(true);
	}

	return true;
}

void MainGame::pause()
{
	Game::pause();

	if (music)
		music->pause();

	map.pause();
	clock.pause();
}

void MainGame::resume()
{
	Game::resume();

	if (music)
		music->play();
	
	map.resume();
	clock.resume();
}

void MainGame::gameTick()
{
	player.HandleKeyInput(map);
	HandleEntranceIntersections();
	HandleItemIntersections();
}

void MainGame::draw()
{
	map.splitDraw(window, "Character", Map::DrawType::Back);
	player.draw(window);
	map.splitDraw(window, "Character", Map::DrawType::Front);

	if (paused && State::IsCurrent(this))
		window.draw(pauseText);
}

void MainGame::HandleItemIntersections()
{
	auto itemLayer = map.GetObjectLayer("Items");
	ObjectSprite *item = nullptr;
	if (itemLayer)
	{
		for (auto object : itemLayer->objects)
		{
			if (player.Intersects(*object))
			{
				item = object;
				break;
			}
		}

		if (item)
		{
			player.TakeItem(item);
			itemLayer->RemoveSprite(item);
		}
	}
}

void MainGame::HandleEntranceIntersections()
{
	auto entranceLayer = map.GetObjectLayer("Entrance");
	ObjectSprite *entrance = nullptr;
	if (entranceLayer)
	{
		for (auto object : entranceLayer->objects)
		{
			if (player.Intersects(*object))
			{
				entrance = object;
				break;
			}
		}

		if (entrance && entrance->ContainsProperty("EntranceTo"))
		{
			auto mapFile = entrance->GetProperty<std::filesystem::path>("EntranceTo");
			auto x = entrance->GetProperty<int>("SpawnX");
			auto y = entrance->GetProperty<int>("SpawnY");
			map.loadRelative(mapFile, State::Textures());
			LoadMusic();

			player.SetPosition(x, y);
		}
	}
}

void MainGame::LoadMusic()
{
	if (!music)
		music = std::make_unique<sf::Music>();
	music->setLoop(true);
	
	std::filesystem::path musicFile;
	if (map.ContainsProperty("Music"))
		musicFile = map.GetPathProperty("Music");

	if (musicFile.has_filename() && music->openFromFile(musicFile.string()))
		music->play();
	else
		music.reset();
}
