#pragma once

#include "TileLayer.h"
#include "ObjectLayer.h"
#include <unordered_map>

namespace Json
{
	class Value;
}

class Map
{
public:
	Map() = default;
	~Map();
	
	void clear();									// Clear the map
	bool load(const std::string &filename);			// Load map from Tiled JSON file
	void draw(sf::RenderWindow &window);			// Draw the map
	TileLayer *GetTileLayer(const std::string &layerName);
	ObjectLayer *GetObjectLayer(const std::string &layerName);
	// TODO impl: GetObjectSprite(int id); // map<int, ObjectSprite *>

private:
	// The owner of the Layer-pointers, also used to draw
	std::vector<Layer*> objects;
	// Different ordering of objects, used as lookup table
	std::unordered_map<std::string, TileLayer *> tileMap;
	std::unordered_map<std::string, ObjectLayer *> objectMap;

	TileSize tileSize;

	std::unordered_map<int, sf::Texture *> tileSets;
	void loadTileSets(Json::Value &root);

	// <animationtileid, animationdata< frame<tileid, duration>> >
	AnimationTileMap animatedTiles;
	void loadAnimatedTiles(int firstGid, Json::Value &tileset);

    // Handles regular layers
	void loadLayer(Json::Value& layer);

	// Handles object layers
	void loadObjects(Json::Value& layer);

	// Shared clock for all animated tiles
	sf::Clock clock;

	// TODO impl: propertymap

	const unsigned int flipMultiplier = pow(2, 30);
};
