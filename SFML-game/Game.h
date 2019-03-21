#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include <SFML/chronometer.h>
#include "map.h"
#include "Player.h"
#include "KeyHandler.hpp"
#include "IntersectionHandler.h"
#include "Music.hpp"

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
	void loadMusic(const MapProperties &map, Music &music);
	void loadProperties(const MapProperties &map);

	// Class object (should be) initialized by App
	sftools::Chronometer m_clock;

	// "Personal" class variables
	Player m_player;

	Music m_music;
	Map m_map;
	sf::Text m_pauseText;

	KeyHandler m_keyHandler;
	IntersectionHandler m_intersectionHandler;
};
