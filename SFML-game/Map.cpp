#include "stdafx.h"
#include "Map.hpp"

#include <fstream>
#include "State.hpp"
#include "TileLayer.hpp"
#include "ObjectLayer.hpp"

void Map::clear()
{
	m_layers.clear();
	m_tileSets.clear();
	m_objectMap.clear();
	m_tileMap.clear();
	m_propertyMap.clear();
	m_currentPath = "";
	m_currentFile = "";

	m_clock.reset(true);
}

bool Map::load(const std::filesystem::path &filename, TextureMap &textures, const ObjectSpriteFactory &spriteFactory)
{
	clear();

	// Will contain the data we read in
	Json::Value root;

	// Parses the file
	Json::Reader reader;

	// Stream used for reading the data file. The data file has been saved as JSON in Tiled
	std::ifstream file(filename);

	// Read data from file into root object
	bool parsingSuccessful = reader.parse(file, root);
	if (!parsingSuccessful)
		return false;

	std::error_code ec;
	m_currentFile = std::filesystem::weakly_canonical(filename, ec);
	m_currentPath = m_currentFile.parent_path();

	// Get tile size information
	tileSize.x = root["tilewidth"].asInt();
	tileSize.y = root["tileheight"].asInt();
	tileSize.s = root["spacing"].asInt();
	width = root["width"].asInt();
	height = root["height"].asInt();

	loadTileSets(root, textures);
	loadProperties(root["properties"]);

	// Read in each layer
	for (Json::Value& layer: root["layers"])
	{
		if (layer["type"] == "tilelayer")
			loadLayer(layer);
		
		else if (layer["type"] == "objectgroup")
			loadObjects(layer, spriteFactory);
	}

	return true;
}

bool Map::save(const std::filesystem::path &filename)
{
	// Will contain the data we save
	Json::Value value;

	// Store attributes from the map itself
	value["version"] = 1.2;
	value["type"] = "map";
	value["orientation"] = "orthogonal";
	value["renderorder"] = "right-down";
	value["infinite"] = false;

	value["width"] = width;
	value["height"] = height;
	value["tilewidth"] = tileSize.x;
	value["tileheight"] = tileSize.y;
	value["spacing"] = tileSize.s;

	saveProperties(value["properties"]);

	for (auto[firstgid, tileset] : m_tileSets)
		tileset.save(value["tilesets"]);

	for (auto &layer : m_layers)
		layer->save(value["layers"]);

	// Stream used for storing the data file as JSON in Tiled
	std::ofstream file(filename);

	// Write the file
	Json::FastWriter fastWriter;
	file << fastWriter.write(value);
	file.close();

	return file.good();
}

void Map::loadLayer(const Json::Value& layer)
{
	// Store info on layer
	auto tmp = static_cast<TileLayer *>(m_layers.emplace_back(
		std::make_unique<TileLayer>(tileSize)).get());

	tmp->load(layer, m_tileSets);
	m_tileMap.try_emplace(tmp->name, tmp);
}

void Map::loadObjects(const Json::Value& layer, const ObjectSpriteFactory &spriteFactory)
{
	// Store info on layer
	auto objectLayer = static_cast<ObjectLayer *>(m_layers.emplace_back(
		std::make_unique<ObjectLayer>(tileSize)).get());
	
	objectLayer->load(layer, m_tileSets, spriteFactory);
	m_objectMap.try_emplace(objectLayer->name, objectLayer);
}

void Map::loadTileSets(const Json::Value &root, TextureMap &textures) // Loads all the images used by the json file as textures
{
	for (auto &val : root["tilesets"])
	{
		TileSet tileset;
		tileset.load(val, m_currentPath, textures);
		m_tileSets.try_emplace(tileset.firstgid, tileset);
	}
}

void Map::draw(sf::RenderTarget &target)
{
	for (auto &layer : m_layers)
		drawLayer(target, layer.get());
}

void Map::drawLayer(sf::RenderTarget &target, Layer *layer)
{
	if (m_clock.isRunning())
		layer->process(m_clock);
	
	if (layer->visible)
		layer->draw(target);
}

void Map::drawFrontOf(sf::RenderTarget &target, const std::string &ofLayer)
{
	bool isDrawing = false;

	for (auto &layer : m_layers)
	{
		if (isDrawing)
			drawLayer(target, layer.get());
		else if (layer->name == ofLayer)
			isDrawing = true;
	}
}

void Map::drawBackOf(sf::RenderTarget &target, const std::string &ofLayer)
{
	for (auto &layer : m_layers)
	{
		if (layer->name == ofLayer)
			break;

		drawLayer(target, layer.get());
	}
}

TileLayer *Map::getTileLayer(const std::string& layerName)
{
	auto it = m_tileMap.find(layerName);
	if (it != m_tileMap.end())
		return it->second;

	return nullptr;
}

const TileLayer *Map::getTileLayer(const std::string& layerName) const
{
	auto it = m_tileMap.find(layerName);
	if (it != m_tileMap.end())
		return it->second;

	return nullptr;
}

ObjectLayer *Map::getObjectLayer(const std::string& layerName)
{
	auto it = m_objectMap.find(layerName);
	if (it != m_objectMap.end())
		return it->second;

	return nullptr;
}

const ObjectLayer *Map::getObjectLayer(const std::string& layerName) const
{
	auto it = m_objectMap.find(layerName);
	if (it != m_objectMap.end())
		return it->second;

	return nullptr;
}

void Map::pause()
{
	m_clock.pause();
}

void Map::resume()
{
	m_clock.resume();
}
