#pragma once

#include "TileLayer.h"
#include "ObjectLayer.h"
#include <unordered_map>

class Map
{
public:
	enum class DrawType{ Front, Back };
	
	Map() = default;
	~Map();
	
	void clear();									// Clear the map
	bool load(const std::string &filename);			// Load map from Tiled JSON file
	void draw(sf::RenderWindow &window);			// Draws the entire map right away
	void splitDraw(sf::RenderWindow &window, const std::string &byLayer, DrawType drawType); // The layer that is split by won't be drawn
	static void drawLayer(sf::RenderWindow &window, Layer *layer); // Draws a single layer in a window
	
	TileLayer *GetTileLayer(const std::string &layerName);
	ObjectLayer *GetObjectLayer(const std::string &layerName);
	// TODO impl: GetObjectSprite(int id); // map<int, ObjectSprite *>

	// The owner of the Layer-pointers, used to draw, check intersections etc.
	std::vector<Layer *> layers;
	bool isWalkableTileCoords(int xPos, int yPos);
	bool isWalkableScreenCoords(int xCoord, int yCoord);
	void SetWalkables();

private:
	// Different ordering of layers, used as lookup table
	std::unordered_map<std::string, TileLayer *> tileMap;
	std::unordered_map<std::string, ObjectLayer *> objectMap;
	
	template <class T>
	T& get(T *arr, int x, int y) { return arr[x + y * width]; }
	bool *walkables = nullptr; // Need to go through the layers afterwards and assign true/false

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

	// Map bounds
	int width, height;

	const unsigned int flipMultiplier = pow(2, 30);
};
