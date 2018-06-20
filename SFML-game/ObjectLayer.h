#pragma once
#include "Layer.h"
#include "ObjectSprite.h"

class ObjectLayer : public Layer
{
	// Map needs to access protected/private data
	friend class Map;

public:
	ObjectLayer(TileSize tileSize, std::unordered_map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles) : Layer(tileSize, tileSets, animatedTiles) { }
	~ObjectLayer();

	void process() override;
	void draw(sf::RenderWindow& window) override;
	void loadTexture() override;

	std::vector<ObjectSprite *> objects;

protected:

	// todo std::map<int gid, ObjectSprite *>
};
