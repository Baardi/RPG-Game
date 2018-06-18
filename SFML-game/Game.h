#pragma once
#include <SFML/Graphics.hpp>
#include "Apple.h"
#include "Snake.h"
#include "Machine.h"
#include "chronometer.h"
#include "UI.h"
#include "map.h"

class GameInitializer : public Initializer
{
public:
	GameInitializer(int players) : players(players){}
	int players;
};

class Game : public UI
{
public:
	Game(sf::RenderWindow &window, sf::Event &event, sf::Font &font);
	~Game();
	void init();
	bool frame() override;
	void pause() override;
	void resume() override;
	void toggle() override;
	void tick();
	void draw();

private:
	//Functions
	void UpdateScore();
	void HandleKeyInput();
	void KillSnake(int pos);

	//int framespertick = 0; // <-- Used for measuring performance

	//Class object initialized by App
	sf::RenderWindow &window;
	sftools::Chronometer clock;
	sf::Font &font;

	//"Personal" class variables
	std::vector<Player*> players;
	
	std::unordered_map<std::string, TileLayer*> layerMap;
	//std::unordered_map<int, ObjectLayer*> spriteMap; todo: group "sub"-objects

	Apple apple;

	sf::Text pauseText;
	bool pausable = false;
	Map *map = nullptr;
};
