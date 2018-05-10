#include "stdafx.h"
#include "Game.h"
#include <iostream>
#include "Machine.h"


Game::Game(sf::RenderWindow &window, sf::Event &event, sf::Font &font) : UI(window, event, font),
                                                       window(window),
                                                       font(font),
                                                       apple(window),
                                                       pauseText("Paused", font, 50)
{
	// Initialize
	GameInitializer* initializer = dynamic_cast<GameInitializer*>(Machine::GetInitializer());
	int numberOfPlayers = initializer ? initializer->players : 1;

	players.push_back(new Player(window, sf::Keyboard::Left, sf::Keyboard::Right, sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255), font, players.size()));
	
	if (players.size() < numberOfPlayers)
		players.push_back(new Player(window, sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255), font, players.size()));

	if (players.size() < numberOfPlayers)
		players.push_back(new Player(window, sf::Keyboard::C, sf::Keyboard::V, sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255), font, players.size()));

	if (players.size() < numberOfPlayers)
		players.push_back(new Player(window, sf::Keyboard::Numpad8, sf::Keyboard::Numpad9, sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255), font, players.size()));
		
	// Color/Keys should (would be nice) be loaded from somewhere (config or struct)
	// Just add 5 items with 3 params as input, and let the new structure handle the rest

	map = new Map();
	if (!map->load("data/Intro village.json", mapObjects))
		Machine::Set(Transition::Reset, State::MainMenu);

	pauseText.setPosition(400, 450);
}

Game::~Game()
{
	for (auto player : players)
		delete player;

	for (auto object : mapObjects) // nothing to delete for now, but might be later
		delete object;

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
	if (!paused && clock.getElapsedTime().asMilliseconds() > 12)
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

	// move part
	for (auto player : players)
	{
		auto snake = player->snake;

		if (!snake)
			continue;

		snake->HandleKeyInput();
		snake->move();
	}

	// collision check part
	for (auto player : players)
	{
		auto snake = player->snake;

		if (!snake)
			continue;

		if (snake->Collides(apple))
		{
			apple.respawn();
			snake->grow();
			player->SetScore();
		}

		for (auto otherPlayer : players)
		{
			auto otherSnake = otherPlayer->snake;

			if (otherSnake && snake->Collides(*otherSnake))
				snake->isAlive = false;
		}
	}

	// kill part
	for (auto player : players)
	{
		auto snake = player->snake;

		if (snake && !snake->isAlive)
		{
			delete snake;
			player->snake = nullptr;
		}
	}

}

void Game::draw()
{
	for (auto object : mapObjects)
		object->draw(window);

	apple.draw();

	for (auto player : players)
	{
		auto snake = player->snake;

		if (snake)
			snake->draw();
	}

	for (auto player : players)
		player->score.draw();

	if (paused)
		window.draw(pauseText);
}

void Game::UpdateScore()
{
	for (auto player : players)
		player->SetScore();
}

void Game::HandleKeyInput()
{
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

void Game::KillSnake(int pos)
{
	delete players[pos]->snake;
	players.erase(players.begin() + pos);
}
