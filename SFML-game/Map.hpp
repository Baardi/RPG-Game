#pragma once

#include <filesystem>
#include "ObjectSprite.hpp"
#include "App/ResourceManager.hpp"

class TileLayer;
class ObjectLayer;
class FringeDrawer;

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
	bool load(const std::filesystem::path &filename, Textures &textures, const ObjectSpriteFactory &spriteFactory = ObjectSpriteFactory());

	// Save to from Tiled JSON file
	bool save(const std::filesystem::path &filename);

	// Clear map in memory
	void clear();									
	
	// Current loaded file
	std::filesystem::path getFile() const { return m_currentFile; }

	// Working directory used by map. Always parent path of current file
	std::filesystem::path getPath() const { return m_currentPath; } 

	void draw(sf::RenderTarget &target);
	void drawWithFringe(sf::RenderTarget &target, const std::string &splitLayer, FringeDrawer &fringeDrawer);
	void drawLayer(sf::RenderTarget &target, Layer *layer, FringeDrawer *fringeDrawer);
	
	TileLayer *getTileLayer(const std::string &layerName);
	const TileLayer *getTileLayer(const std::string &layerName) const;
	ObjectLayer *getObjectLayer(const std::string &layerName);
	const ObjectLayer *getObjectLayer(const std::string &layerName) const;
	
	// TODO impl: GetObjectSprite(int id); // unordered_map<int, ObjectSprite *>

	void pause();
	void resume();

	// The owner of the Layer-pointers, used to draw
	std::vector<std::unique_ptr<Layer>> m_layers; // TODO: Refactor into tree

	// Sorted collection of layers
	std::unordered_map<std::string, ObjectLayer *> m_objectMap;
	std::unordered_map<std::string, TileLayer *> m_tileMap;

	// Map bounds
	unsigned int width, height;

	TileSize tileSize;

	std::optional<sf::Color> backgroundColor;
	sf::RectangleShape maprect;
private:

	std::unordered_map<int, TileSet> m_tileSets;
	void loadTileSets(const Json::Value &root, Textures &textures);

	// Handles group of layers
	void loadLayers(const Json::Value& layer, Textures &textures, const ObjectSpriteFactory& spriteFactory);

    // Handles regular layers
	void loadTileLayer(const Json::Value& layer);

	// Handles object layers
	void loadObjects(const Json::Value& layer, const ObjectSpriteFactory &spriteFactory);

	// Handles image layers
	void loadImageLayer(const Json::Value& layer, Textures &textures);

	// Shared clock for all animated tiles
	sftools::Chronometer m_clock;
	 
	std::filesystem::path m_currentFile;
	std::filesystem::path m_currentPath;

public:
	static constexpr unsigned int FLIP_MULTIPLIER = 0b1 << 30; /* 1073741824*/
};
