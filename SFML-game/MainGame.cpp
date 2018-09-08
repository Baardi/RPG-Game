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
	UI::init();

	map.load("data/Intro village.json");
	LoadMusic();
	// Need a better solution
		//State::Set<MainMenu>(Transition::Switch);
}

bool MainGame::frame()
{
	if (!paused)
	{
		gameTick();
		clock.reset(true);
	}

	HandleKeyInput();
	draw();

	return true;
}

void MainGame::pause()
{
	if (music)
		music->pause();

	map.pause();
	clock.pause();
	paused = true;
}

void MainGame::resume()
{
	if (music)
		music->play();
	
	map.resume();
	clock.resume();
	paused = false;
	pausable = false;
}

void MainGame::toggle()
{
	if (!pausable)
		return;

	paused ? resume() : pause();
	pausable = false;
}

void MainGame::gameTick()
{
	player.HandleKeyInput(map);
	HandleEntranceIntersections();
}

void MainGame::draw()
{
	map.splitDraw(window, "Character", Map::DrawType::Back);
	player.draw(window);
	map.splitDraw(window, "Character", Map::DrawType::Front);

	if (paused && State::IsCurrent(this))
		window.draw(pauseText);
}

void MainGame::HandleKeyInput()
{
	if (!pausable && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)))
		pausable = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
	{
		State::Reset<MainMenu>();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		State::Push<MainMenu>();
	}
	else if (pausable && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		State::Switch<MainGame>();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
	{
		State::PushChild<GamePopupMenu>();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		toggle();
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

		if (entrance)
		{
			auto mapFileName = entrance->GetProperty<std::string>("EntranceTo");
			auto x = entrance->GetProperty<int>("SpawnX");
			auto y = entrance->GetProperty<int>("SpawnY");
			map.load(mapFileName);
			LoadMusic();

			player.SetPosition(x, y);
		}
	}
}

void MainGame::LoadMusic()
{
	music = std::make_unique<sf::Music>();
	music->setLoop(true);
	std::filesystem::path musicFile;

	if (map.ContainsProperty("Music"))
		musicFile = map.GetProperty<std::filesystem::path>("Music");

	if (music->openFromFile(musicFile.string()))
		music->play();
	else
		music.reset();
}
