#pragma once

#include "TileLayer.h"
#include "ObjectLayer.h"
#include <unordered_map>

namespace Json
{
	class Value;
}

// Class with a single public function that loads a map into an object list
class Map
{
public:
	Map() = default;
	~Map();
	// Load map from Tiled JSON file
	bool load(const std::string &filename);
	void draw(sf::RenderWindow &window);
	TileLayer *GetTileLayer(const std::string &layerName);
	ObjectLayer *GetObjectLayer(const std::string &layerName);

private:
	// The owner of the Layer-pointers, also used to draw
	std::vector<Layer*> objects;
	// Different ordering of objects, used as lookup table
	std::unordered_map<std::string, TileLayer*> tileMap;
	std::unordered_map<std::string, ObjectLayer*> objectMap;

	std::unordered_map<int, sf::Texture *> tileSets;
	void loadTileSets(Json::Value &root);

	// <animationtileid, animationdata< frame<tileid, duration>> >
	std::unordered_map<int, std::vector< std::pair<int, int>> > animatedTiles;
	void loadAnimatedTiles(int firstGid, Json::Value &tileset);

    // Handles regular layers
	void loadLayer(Json::Value& layer, std::vector<Layer*>& objects, TileSize tileSize);

	// Handles object layers
	void loadObjects(Json::Value& layer, std::vector<Layer*>& objects, TileSize tileSize);
};
