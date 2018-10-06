#pragma once
#include "Layer.h"
#include "ObjectSprite.h"

class ObjectLayer : public Layer, public MapProperties
{
public:
	ObjectLayer(const TileSize &tileSize, std::map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles) : Layer(tileSize, tileSets, animatedTiles) { }
	~ObjectLayer();

	void load(Json::Value &layer, sftools::Chronometer &clock);
	void process() override;
	void draw(sf::RenderWindow& window) override;
	void loadTexture() override;
	void RemoveSprite(ObjectSprite *sprite);
	ObjectSprite *GetIntersectedObject(const GameObject &other);

	std::vector<ObjectSprite *> objects;

	// todo std::map<int gid, ObjectSprite *>
};
