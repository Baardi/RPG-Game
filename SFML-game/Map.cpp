#include "stdafx.h"
#include "Map.hpp"

#include <fstream>
#include "TileLayer.hpp"
#include "ObjectLayer.hpp"
#include "ImageLayer.hpp"
#include "SfUtility.hpp"
#include "FringeDrawer.hpp"

void Map::clear()
{
	m_layers.clear();
	m_tileSets.clear();
	m_objectMap.clear();
	m_tileMap.clear();
	m_currentFile.clear();
	m_currentPath.clear();

	clearProperties();
	m_clock.reset(true);
}

bool Map::load(const std::filesystem::path &filename, std::map<std::string, sf::Texture> &textures, const ObjectSpriteFactory &spriteFactory)
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
	m_currentFile = std::filesystem::weakly_canonical(filename, ec); // NOTE: weakly_canonical has some issues with mapped drives. Doesn't matter for me though
	m_currentPath = m_currentFile.parent_path();

	// Get tile size information
	tileSize.x = root["tilewidth"].asInt();
	tileSize.y = root["tileheight"].asInt();
	width = root["width"].asInt();
	height = root["height"].asInt();

	maprect.setSize(sf::Vector2f(width*tileSize.x, height*tileSize.y));
	if (!root["backgroundcolor"].empty())
	{
		backgroundColor = sf::utility::parseColor(root["backgroundcolor"].asString());
		maprect.setFillColor(*backgroundColor);
	}
		
	// Load tilets
	loadTileSets(root, textures);

	// Load properties
	loadProperties(root["properties"]);
	
	// Read in each layer
	m_layers.reserve(root["layers"].size());
	loadLayers(root["layers"], textures, spriteFactory);

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

	if (backgroundColor.has_value())
		value["backgroundcolor"] = sf::utility::serializeColor(*backgroundColor);

	// Save properties
	saveProperties(value["properties"]);

	// Save tilesets
	for (auto[firstgid, tileset] : m_tileSets)
		tileset.save(value["tilesets"]);

	// Save layers
	for (auto &layer : m_layers)
		layer->save(value["layers"]);

	// Stream used for storing the data file as JSON in Tiled
	std::ofstream file(filename);

	// Write the file
	Json::FastWriter fastWriter;
	fastWriter.omitEndingLineFeed();
	file << fastWriter.write(value);
	file.close();

	return file.good();
}

void Map::loadTileSets(const Json::Value& root, std::map<std::string, sf::Texture>& textures) // Loads all the images used by the json file as textures
{
	for (auto& val : root["tilesets"])
	{
		TileSet tileset;
		tileset.load(val, m_currentPath, textures);
		m_tileSets.try_emplace(tileset.firstgid, std::move(tileset));
	}
}

void Map::loadLayers(const Json::Value& layers, std::map<std::string, sf::Texture>& textures, const ObjectSpriteFactory& spriteFactory)
{
	for (const Json::Value& layer : layers)
	{
		const auto layerType = layer["type"].asString();

		if (layerType == "tilelayer")
			loadTileLayer(layer);
		else if (layerType == "objectgroup")
			loadObjects(layer, spriteFactory);
		else if (layerType == "imagelayer")
			loadImageLayer(layer, textures);
		else if (layerType == "group")
			loadLayers(layer["layers"], textures, spriteFactory); // Dumb flat hiearchy, but drawOrder is still preserved
	}
}

void Map::loadTileLayer(const Json::Value& layer)
{
	// Store info on layer
	auto tmp = std::make_unique<TileLayer>(tileSize);
	tmp->load(layer, m_tileSets);

	m_tileMap.try_emplace(tmp->name, tmp.get());
	m_layers.push_back(std::move(tmp));
}

void Map::loadObjects(const Json::Value& layer, const ObjectSpriteFactory &spriteFactory)
{
	// Store info on layer
	auto objectLayer = std::make_unique<ObjectLayer>(tileSize);
	objectLayer->load(layer, m_tileSets, spriteFactory);
	
	m_objectMap.try_emplace(objectLayer->name, objectLayer.get());
	m_layers.push_back(std::move(objectLayer));
}

void Map::loadImageLayer(const Json::Value &layer, std::map<std::string, sf::Texture> &textures)
{
	// Store info on layer
	auto imageLayer = std::make_unique<ImageLayer>(tileSize);
	imageLayer->load(layer, m_currentPath, textures);

	m_layers.push_back(std::move(imageLayer));
}

void Map::draw(sf::RenderTarget &target)
{
	if (backgroundColor.has_value())
		target.draw(maprect);

	for (auto &layer : m_layers)
		drawLayer(target, layer.get(), nullptr);
}

void Map::drawWithFringe(sf::RenderTarget &target, const std::string & splitLayer, FringeDrawer &fringeDrawer)
{
	if (backgroundColor.has_value())
		target.draw(maprect);

	for (auto &layer : m_layers)
	{
		if (layer->name == splitLayer)
			drawLayer(target, layer.get(), &fringeDrawer);
		else
			drawLayer(target, layer.get(), nullptr);
	}
}

void Map::drawLayer(sf::RenderTarget &target, Layer *layer, FringeDrawer *fringeDrawer)
{
	if (m_clock.isRunning())
		layer->process(m_clock);
	
	if (layer->visible)
	{
		if (fringeDrawer)
			layer->drawWithFringe(target, *fringeDrawer);
		else
			layer->draw(target);
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
