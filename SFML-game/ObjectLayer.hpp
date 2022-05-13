#pragma once
#include "Layer.hpp"
#include "ObjectSprite.hpp"

class ObjectLayer : public Layer
{
public:
	ObjectLayer(TileSize tileSize) : Layer(tileSize) { }
	~ObjectLayer() = default;

	bool load(const Json::Value &layer, const std::map<int, TileSet> &tileSets, const ObjectSpriteFactory &spriteFactory);
	bool save(Json::Value &layers) const override;
	
	void process(const sftools::Chronometer &clock) override;
	void draw(sf::RenderTarget &target) override;
	void loadTexture(const std::map<int, TileSet> &tileSets) override;
	void removeSprite(ObjectSprite *sprite);
	ObjectSprite *getIntersectedObject(const GameObject &other);

	std::vector<std::unique_ptr<ObjectSprite>> objects;
	// todo std::map<int gid, ObjectSprite *>
};
