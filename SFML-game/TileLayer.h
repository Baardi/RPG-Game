#pragma once
#include "Layer.h"
#include "chronometer.h"

// Class representing a tile layer
class TileLayer : public Layer
{
	// Map needs to access protected/private data
	friend class Map;

public:
	TileLayer(TileSize tileSize, std::unordered_map<int, sf::Texture *> &tileSets, std::unordered_map<int, std::vector<std::pair<int, int>>> animatedTiles) : Layer(tileSize, tileSets, animatedTiles) {  }

	void draw(sf::RenderWindow& window) override;
	void process() override;
	void loadTexture() override;

private:

    // Lazy, but ram is cheap!
    int tilemap[30][30];
	AnimationTile animationTilemap[30][30];
	sf::Sprite textureMap[30][30];

    // Size in tiles
	int width, height;

	// Todo: Make the clock more shared
	sf::Clock clock;
};
