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

	// Virtual overriden methods 
	void init() override;
	bool frame() override;
	void pause() override;
	void resume() override;
	void draw() override;

private:
	// Private Methods
	void HandleItemIntersections(ObjectLayer *layer, ObjectSprite *item);
	void HandleEntranceIntersections(ObjectLayer *layer, ObjectSprite *entrance);
	void LoadMusic(const MapProperties &map, Music &music);
	void LoadProperties(const MapProperties &map);

	// Class object (should be) initialized by App
	sftools::Chronometer clock;

	// "Personal" class variables
	Player player;

	Music m_music;
	Map map;
	sf::Text pauseText;

	KeyMapper keyMapper;
	IntersectionHandler intersectionHandler;
};
