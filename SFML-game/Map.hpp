#pragma once

#include "State.hpp"
#include "MapProperties.hpp"
#include <filesystem>
#include "Layer.hpp"

class TileLayer;
class ObjectLayer;

class Map : public MapProperties
{
public:	
	Map() = default;
	~Map() = default;
	
	Map(const Map &) = delete;
	Map &operator=(const Map &) = delete;

	Map(Map &&) = default;
	Map &operator=(Map &&) = default;
	
	// Load map from Tiled JSON file
	bool load(const std::filesystem::path &filename, TextureMap &textures); 
	void clear();									
	
	auto getPath() const { return m_currentPath; }
	
	void draw(sf::RenderTarget &target);
	void drawFrontOf(sf::RenderTarget &target, const std::string &ofLayer);
	void drawBackOf(sf::RenderTarget &target, const std::string &ofLayer);
	void drawLayer(sf::RenderTarget &target, Layer *layer);
	
	TileLayer *getTileLayer(const std::string &layerName);
	const TileLayer *getTileLayer(const std::string &layerName) const;
	ObjectLayer *getObjectLayer(const std::string &layerName);
	const ObjectLayer *getObjectLayer(const std::string &layerName) const;
	
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
