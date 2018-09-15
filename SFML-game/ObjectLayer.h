#pragma once
#include "Layer.h"
#include "ObjectSprite.h"

class ObjectLayer : public Layer, public MapProperties
{
	// Map needs to access protected/private data
	friend class Map;

public:
	ObjectLayer(const TileSize &tileSize, std::map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles) : Layer(tileSize, tileSets, animatedTiles) { }
	~ObjectLayer();

	void process() override;
	void draw(sf::RenderWindow& window) override;
	void loadTexture() override;

	std::vector<ObjectSprite *> objects;

	// todo std::map<int gid, ObjectSprite *>
};
