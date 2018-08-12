#include "stdafx.h"
#include "MainGame.h"
#include <iostream>
#include "State.h"
#include "MainMenu.h"
#include "GamePopupMenu.h"

MainGame::MainGame(): player(clock), pauseText("Paused", font, 50)
{
	pauseText.setPosition(400, 450);
}

MainGame::~MainGame()
{
	if (map)
		delete map;
}

void MainGame::init()
{
	UI::init();
	
	map = new Map();
	if (!map->load("data/Intro village.json"))
		State::Set<MainMenu>(Transition::Switch);
}

bool MainGame::frame()
{
	//TODO: tick speed and jump amount to be moved outwards, to config file
	//framespertick++;
	if (!paused)
	{
		//std::cout << framespertick << std::endl;
		//framespertick = 0;

		tick();
		clock.reset(true);
	}

	HandleKeyInput();
	draw();

	return true;
}

void MainGame::pause()
{
	clock.pause();
	paused = true;
}

void MainGame::resume()
{
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

void MainGame::tick()
{
	auto entranceLayer = map->GetObjectLayer("Entrance");
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
			std::string mapFileName = entrance->GetPropertyValue("EntranceTo");
			int x = atoi(entrance->GetPropertyValue("SpawnX").c_str());
			int y = atoi(entrance->GetPropertyValue("SpawnY").c_str());

			map->load(mapFileName);
			player.SetPosition(x, y);
		}
	}
}

void MainGame::draw()
{
	map->draw(window);
	player.draw(window);

	if (paused && State::IsCurrent(this))
		window.draw(pauseText);
}

void MainGame::HandleKeyInput()
{
	player.HandleKeyInput();

	if (!pausable && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)))
		pausable = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
	{
		State::Set<MainMenu>(Transition::Switch);
		State::ClearInitializer();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		State::Set<MainMenu>();
		State::ClearInitializer();
	}
	else if (pausable && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		State::Set<MainGame>(Transition::Switch);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
	{
		State::SetChild<GamePopupMenu>();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		toggle();
}
