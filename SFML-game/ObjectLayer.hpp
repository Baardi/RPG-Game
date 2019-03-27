#pragma once
#include "Layer.hpp"
#include "ObjectSprite.hpp"

class ObjectLayer : public Layer
{
public:
	ObjectLayer(const TileSize &tileSize) : Layer(tileSize) { }
	~ObjectLayer();

	void load(const Json::Value &layer, const std::map<int, TileSet> &tileSets, const ObjectSpriteFactory &spriteFactory);
	void save(Json::Value &layers) const override;
	
	void process(const sftools::Chronometer &clock) override;
	void draw(sf::RenderTarget &target) override;
	void loadTexture(const std::map<int, TileSet> &tileSets) override;
	void removeSprite(ObjectSprite *sprite);
	ObjectSprite *getIntersectedObject(const GameObject &other);

	std::vector<std::unique_ptr<ObjectSprite>> objects;
	// todo std::map<int gid, ObjectSprite *>
};
