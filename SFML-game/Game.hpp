#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "KeyHandler.hpp"
#include "IntersectionHandler.hpp"
#include "Music.hpp"
#include "ObjectFactory.hpp"
#include "App/State.hpp"
#include "FringeDrawer.hpp"

namespace appstate {

class Game : public appstate::State
{
public:
	Game();
	~Game();

	// Virtual overriden methods 
	void init() override;
	void pause() override;
	void resume() override;
	bool frame() override;
	void draw(sf::RenderTarget &target) override;

	void updateDrawRect(); // player needs access to this
private:
	// Private Methods
	bool loadMap(const std::filesystem::path &mapFile);

	void loadMusic(const MapProperties &map, Music &music);
	void loadProperties(const MapProperties &map);

	// Class object (should be) initialized by App
	sftools::Chronometer m_clock;

	// "Personal" class variables
	Player m_player;
	FringeDrawer m_fringeDrawer;

	Music m_music;
	Map m_map;
	sf::Text m_pauseText;

	KeyHandler m_keyHandler;
	IntersectionHandler m_intersectionHandler;

	GameItemFactory m_itemFactory;
	ObjectSpriteFactory m_spriteFactory;

	sf::RenderTexture m_renderTexture;
	std::optional<sf::Sprite> m_renderSprite;
};

}
