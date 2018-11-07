#pragma once
#include "Layer.h"
#include "ObjectSprite.h"

template <class T>
class uniquepointerFinder
{
	bool operator()(T *lhs, T *rhs) const
	{
		return lhs() < rhs.get();
	}
};

class ObjectLayer : public Layer, public MapProperties
{
public:
	ObjectLayer(const TileSize &tileSize, std::map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles) : Layer(tileSize, tileSets, animatedTiles) { }
	~ObjectLayer();

	void load(const Json::Value &layer, sftools::Chronometer &clock);
	void process() override;
	void draw(sf::RenderTarget& window) override;
	void loadTexture() override;
	void RemoveSprite(ObjectSprite *sprite);
	ObjectSprite *GetIntersectedObject(const GameObject &other);

	std::vector<std::unique_ptr<ObjectSprite>> objects;

	// todo std::map<int gid, ObjectSprite *>
};
