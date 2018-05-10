#pragma once
#include "MapObject.h"
#include "chronometer.h"

// Class representing a tile layer
class Layer : public MapObject
{
	// Map needs to access protected/private data
	friend class Map;

public:
	Layer(TileSize tileSize, std::map<int, sf::Texture *> &tileSets, std::map<int, std::vector<std::pair<int, int>>> animatedTiles) : MapObject(tileSize, tileSets, animatedTiles) { clock.reset(true); }

	void draw(sf::RenderWindow& window) override;
	void loadTexture() override;

    // Lazy, but ram is cheap!
    int tilemap[100][100];
	AnimationTile animationTilemap[100][100];
	sf::Sprite textureMap[100][100];

protected:

	//void GetTileAnimation();

    // Size in tiles
	int width, height;
    std::string name;

	sftools::Chronometer clock;
};
