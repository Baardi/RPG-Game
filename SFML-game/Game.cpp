#include "stdafx.h"
#include "Game.h"
#include <iostream>
#include "Machine.h"


Game::Game(sf::RenderWindow &window, sf::Event &event, sf::Font &font) : UI(window, event, font),
                                                       window(window),
                                                       font(font),
                                                       pauseText("Paused", font, 50)
{
	map = new Map();
	if (!map->load("data/Intro village.json")) // route 1 is fucked
	{
		Machine::Set(Transition::Reset, State::MainMenu);
		return;
	}

	pauseText.setPosition(400, 450);
}

Game::~Game()
{
	if (map)
		delete map;
}

void Game::init()
{
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
		Machine::Set(Transition::Switch, State::MainMenu);
		Machine::ClearInitializer();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		Machine::Set(Transition::Push, State::MainMenu);
		Machine::ClearInitializer();
	}
	else if (pausable && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		Machine::Set(Transition::Switch, State::Game);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		toggle();
}
