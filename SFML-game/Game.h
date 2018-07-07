#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include <SFML/chronometer.h>
#include "UI.h"
#include "map.h"
#include "Player.h"

class GameInitializer : public Initializer
{
public:
	GameInitializer(int players) : players(players){}
	int players;
};

class Game : public UI
{
public:
	Game();
	~Game();
	void init();
	
	// Todo: frame/tick needs some kind of merge
	bool frame() override;
	void tick();

	void pause() override;
	void resume() override;
	void toggle() override;
	void draw() override;

private:
	//Functions
	void HandleKeyInput();

	//int framespertick = 0; // <-- Used for measuring performance

	//Class object (should be) initialized by App
	sftools::Chronometer clock;

	//"Personal" class variables
	Player player;

	std::unordered_map<std::string, TileLayer*> layerMap;
	//std::unordered_map<int, ObjectLayer*> spriteMap; todo: group "sub"-objects

	sf::Text pauseText;
	bool pausable = false;
	Map *map = nullptr;
};
