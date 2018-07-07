#include "stdafx.h"
#include "Game.h"
#include <iostream>
#include "State.h"
#include "MainMenu.h"

Game::Game(): player(clock)
{
}

Game::~Game()
{
	if (map)
		delete map;
}

void Game::init()
{
	UI::init();
	
	pauseText.setPosition(400, 450);
	pauseText.setString("Paused");
	pauseText.setFont(font);
	pauseText.setCharacterSize(50);

	map = new Map();
	if (!map->load("data/Intro village.json"))
		State::Set(Transition::Switch, new MainMenu);
}

bool Game::frame()
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

void Game::pause()
{
	clock.pause();
	paused = true;
}

void Game::resume()
{
	clock.resume();
	paused = false;
	pausable = false;
}

void Game::toggle()
{
	if (!pausable)
		return;

	paused ? resume() : pause();
	pausable = false;
}

void Game::tick()
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

void Game::draw()
{
	map->draw(window);
	player.draw(window);

	if (paused)
		window.draw(pauseText);
}

void Game::HandleKeyInput()
{
	player.HandleKeyInput();

	if (!pausable && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)))
		pausable = true;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
	{
		State::Set(Transition::Switch, new MainMenu);
		State::ClearInitializer();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		State::Set(Transition::Push, new MainMenu);
		State::ClearInitializer();
	}
	else if (pausable && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		State::Set(Transition::Switch, new Game);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		toggle();
}
