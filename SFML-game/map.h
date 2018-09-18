#pragma once

#include "TileLayer.h"
#include "ObjectLayer.h"
#include "MapProperties.h"

class Map : public MapProperties
{
public:
	enum class DrawType{ Front, Back };
	
	Map() = default;
	~Map();
	
	void clear();									// Clear the map
	bool load(const std::string &filename, std::map<std::string, sf::Texture*> &textures);			// Load map from Tiled JSON file
	void draw(sf::RenderWindow &window);			// Draws the entire map right away
	void splitDraw(sf::RenderWindow &window, const std::string &byLayer, DrawType drawType); // The layer that is split by won't be drawn
	static void drawLayer(sf::RenderWindow &window, Layer *layer); // Draws a single layer in a window
	
	TileLayer *GetTileLayer(const std::string &layerName);
	ObjectLayer *GetObjectLayer(const std::string &layerName);
	
	// TODO impl: GetObjectSprite(int id); // map<int, ObjectSprite *>

	void pause();
	void resume();

	// The owner of the Layer-pointers, used to draw, check intersections etc.
	std::vector<Layer *> layers;

private:
	// Different ordering of layers, used as lookup table
	std::map<std::string, TileLayer *> tileMap;
	std::map<std::string, ObjectLayer *> objectMap;
	TileSize tileSize;

	std::map<int, sf::Texture *> tileSets;
	void loadTileSets(Json::Value &root, std::map<std::string, sf::Texture*> &textures);

	// <animationtileid, animationdata< frame<tileid, duration>> >
	AnimationTileMap animatedTiles;
	void loadAnimatedTiles(int firstGid, Json::Value &tileset);

    // Handles regular layers
	void loadLayer(Json::Value& layer);

	// Handles object layers
	void loadObjects(Json::Value& layer);

	// Shared clock for all animated tiles
	 sftools::Chronometer clock;

	// Map bounds
	int width, height;

	static constexpr unsigned int flipMultiplier = 1073741824; /*std::pow(2, 30)*/
};
