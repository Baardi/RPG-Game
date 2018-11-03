#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include <SFML/chronometer.h>
#include "map.h"
#include "Player.h"
#include "KeyMapper.h"
#include "IntersectionHandler.h"
#include "Music.hpp"

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
	void init() override;
	
	// Todo: frame/gameTick needs some kind of merge
	bool frame() override;

	void pause() override;
	void resume() override;
	//void toggle() override;
	void draw() override;

private:
	//Functions
	void HandleItemIntersections(ObjectLayer *layer, ObjectSprite *item);
	void HandleEntranceIntersections(ObjectLayer *layer, ObjectSprite *entrance);
	void LoadMusic(const MapProperties &map, Music &music);
	void LoadProperties(const MapProperties &map);
	//int framespertick = 0; // <-- Used for measuring performance

	//Class object (should be) initialized by App
	sftools::Chronometer clock;

	//"Personal" class variables
	Player player;

	std::map<std::string, TileLayer*> layerMap;
	//std::map<int, ObjectLayer*> spriteMap; todo: group "sub"-layers

	Music m_music;

	sf::Text pauseText;
	Map map;
	KeyMapper keyMapper;
	IntersectionHandler intersectionHandler;
};
