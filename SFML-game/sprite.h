#pragma once
#include "MapObject.h"

class Sprite : public MapObject
{
	// Map needs to access protected/private data
	friend class Map;

public:
	Sprite(TileSize tileSize, std::map<int, sf::Texture *> &tileSets, std::map<int, std::vector<std::pair<int, int>>> animatedTiles) : MapObject(tileSize, tileSets, animatedTiles) { }

	void process(float time) override;
	void draw(sf::RenderWindow& window) override;
	void loadTexture() override;

protected:
	// Id of first tile
	int id;

	// Location on screen
	int x, y;

	// AnimationData
	AnimationTile animationTileInfo;

	// Times the animation
	sf::Clock clock;

	sf::Sprite sprite;
};
