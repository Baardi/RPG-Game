#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include <SFML/chronometer.h>
#include "map.h"
#include "Player.h"
#include "game.h"

class GameInitializer : public Initializer
{
public:
	GameInitializer(int players) : players(players){}
	int players;
};

class MainGame : public Game
{
public:
	MainGame();
	~MainGame();
	void init() override;
	
	// Todo: frame/gameTick needs some kind of merge
	bool frame() override;
	void gameTick();

	void pause() override;
	void resume() override;
	void toggle() override;
	void draw() override;

private:
	//Functions
	void HandleKeyInput();
	void HandleItemIntersections();
	void HandleEntranceIntersections();
	void LoadMusic();

	//int framespertick = 0; // <-- Used for measuring performance

	//Class object (should be) initialized by App
	sftools::Chronometer clock;

	//"Personal" class variables
	Player player;

	std::map<std::string, TileLayer*> layerMap;
	//std::map<int, ObjectLayer*> spriteMap; todo: group "sub"-layers
	std::unique_ptr<sf::Music> music;

	sf::Text pauseText;
	bool pausable = false;
	Map map;
};
