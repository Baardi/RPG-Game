#pragma once
#include "Layer.hpp"
#include "ObjectSprite.hpp"

class ObjectLayer : public Layer
{
public:
	ObjectLayer(const TileSize &tileSize) : Layer(tileSize) { }
	~ObjectLayer();

	void load(const Json::Value &layer, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles);
	void save(Json::Value &layer) const override;
	
	void process(const sftools::Chronometer &clock) override;
	void draw(sf::RenderTarget &target) override;
	void loadTexture(std::map<int, sf::Texture*> &tileSets, AnimationTileMap &animatedTiles) override;
	void removeSprite(ObjectSprite *sprite);
	ObjectSprite *getIntersectedObject(const GameObject &other);

	std::vector<std::unique_ptr<ObjectSprite>> objects;
	// todo std::map<int gid, ObjectSprite *>
};
