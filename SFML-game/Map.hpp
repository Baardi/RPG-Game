#pragma once

#include "TileLayer.hpp"
#include "ObjectLayer.hpp"
#include "MapProperties.hpp"
#include "State.hpp"
#include <filesystem>

class Map : public MapProperties
{
public:
	enum class DrawType{ Front, Back };
	
	Map() = default;
	~Map() = default;
	
	Map(const Map &) = delete;
	Map &operator=(const Map &) = delete;

	Map(Map &&) = default;
	Map &operator=(Map &&) = default;

	void clear();									// Clear the map
	bool load(const std::filesystem::path &filename, TextureMap &textures);			// Load map from Tiled JSON file
	auto getPath() const { return m_currentPath; }
	void draw(sf::RenderTarget &window);			// Draws the entire map right away
	void splitDraw(sf::RenderTarget &window, const std::string &byLayer, DrawType drawType); // The layer that is split by won't be drawn
	void drawLayer(sf::RenderTarget &window, Layer *layer); // Draws a single layer in a window
	
	TileLayer *getTileLayer(const std::string &layerName);
	ObjectLayer *getObjectLayer(const std::string &layerName);
	
	// TODO impl: GetObjectSprite(int id); // map<int, ObjectSprite *>

	void pause();
	void resume();

	// The owner of the Layer-pointers, used to draws
	std::vector<std::unique_ptr<Layer>> m_layers;

	// Sorted collection of layers
	std::map<std::string, ObjectLayer *> m_objectMap;
	std::map<std::string, TileLayer *> m_tileMap;

private:
	TileSize tileSize;

	std::map<int, sf::Texture *> tileSets;
	void loadTileSets(const Json::Value &root, TextureMap &textures);

	// <animationtileid, animationdata< frame<tileid, duration>> >
	AnimationTileMap animatedTiles;
	void loadAnimatedTiles(int firstGid, const Json::Value &tileset);

    // Handles regular layers
	void loadLayer(const Json::Value& layer);

	// Handles object layers
	void loadObjects(const Json::Value& layer);

	// Shared clock for all animated tiles
	 sftools::Chronometer m_clock;

	// Map bounds
	int m_width, m_height;

	std::filesystem::path m_currentPath;
public:
	static constexpr unsigned int FLIP_MULTIPLIER = 1073741824; /*std::pow(2, 30)*/
};
