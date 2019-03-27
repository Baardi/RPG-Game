#pragma once
#include "State.hpp"
#include "map.hpp"
#include "Player.hpp"
#include "KeyHandler.hpp"
#include "IntersectionHandler.hpp"
#include "Music.hpp"
#include "ObjectFactory.hpp"

class Game : public UI
{
public:
	Game();
	~Game();

	// Virtual overriden methods 
	void init() override;
	void pause() override;
	void resume() override;
	bool frame(sf::Window &window) override;
	void draw(sf::RenderTarget &target) override;

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

	GameItemFactory m_itemFactory;
	ObjectSpriteFactory m_spriteFactory;
};
