#pragma once

#include "layer.h"
#include "sprite.h"

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
	bool load(std::string filename, std::vector<MapObject*>& objects);
	void clear_arrays();

    Layer* box_layer = nullptr;
    Layer* brick_layer = nullptr;
    Layer* powerup_layer = nullptr;

	int explosion[15][15], bombs[15][15];

private:

	std::map<int, sf::Texture *> tileSets;
	void loadTileSets(Json::Value &root);

	// |animationtileid|animationdata|frame|tileid|duration|
	std::map<int, std::vector<std::pair<int, int>>> animatedTiles;
	void loadAnimatedTiles(int firstGid, Json::Value &tileset);

	void loadAnimatedObjects(int firstGid, Json::Value &tileset);

    // Handles regular layers
	void loadLayer(Json::Value& layer, std::vector<MapObject*>& objects, TileSize tileSize);

	// Handles object layers
	void loadObjects(Json::Value& root, Json::Value& layer, std::vector<MapObject*>& objects, TileSize tileSize);
};
