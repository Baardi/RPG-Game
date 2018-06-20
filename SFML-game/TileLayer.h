#pragma once
#include "Layer.h"

// Class representing a tile layer
class TileLayer : public Layer
{
	// Map needs to access protected/private data
	friend class Map;

public:
	TileLayer(TileSize tileSize, std::unordered_map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles, sf::Clock &clock) : Layer(tileSize, tileSets, animatedTiles), clock(clock) {  }

	void draw(sf::RenderWindow& window) override;
	void process() override;
	void loadTexture() override;

private:

    // Lazy, but ram is cheap!
	// 31x31 isntead of 30x30 temporarily to make Route 1 work
    int tilemap[31][31];
	AnimationTile animationTilemap[31][31];
	sf::Sprite textureMap[31][31];

    // Size in tiles
	int width, height;

	sf::Clock &clock;
};
