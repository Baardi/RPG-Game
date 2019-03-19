#include "stdafx.h"
#include <fstream>
#include "map.h"
#include <filesystem>
#include "State.h"

void Map::clear()
{
	m_layers.clear();
	tileSets.clear();
	m_objectMap.clear();
	m_tileMap.clear();
	animatedTiles.clear();
	m_propertyMap.clear();
	m_currentPath = "";

	m_clock.reset(true);
}

bool Map::load(const std::filesystem::path &filename, TextureMap &textures)
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

	m_currentPath = std::filesystem::canonical(filename).parent_path();

	// Get tile size information
	tileSize.x = root["tilewidth"].asInt();
	tileSize.y = root["tileheight"].asInt();
	tileSize.s = root["spacing"].asInt();
	m_width = root["width"].asInt();
	m_height = root["height"].asInt();

	loadTileSets(root, textures);

	loadProperties(root);

	// Read in each layer
	for (Json::Value& layer: root["layers"])
	{
		if (layer["type"] == "tilelayer")
			loadLayer(layer);
		
		else if (layer["type"] == "objectgroup")
			loadObjects(layer);
	}

	return true;
}

void Map::loadLayer(const Json::Value& layer)
{
	// Store info on layer
	auto tmp = static_cast<TileLayer *>(m_layers.emplace_back(
		std::make_unique<TileLayer>(tileSize))
		.get());				   // vector, so the order is kept

	tmp->load(layer, tileSets, animatedTiles);
	m_tileMap.try_emplace(tmp->name, tmp);   // so the layer can be retrieved later (e.g by game-class)
}

void Map::loadObjects(const Json::Value& layer)
{
	// Store info on layer
	auto objectLayer = static_cast<ObjectLayer *>(m_layers.emplace_back(
		std::make_unique<ObjectLayer>(tileSize))
		.get());				   // vector, so the order is kept
	
	objectLayer->load(layer, m_clock, tileSets, animatedTiles);
	m_objectMap.try_emplace(objectLayer->name, objectLayer); // so the layer can be retrieved later (e.g by game-class)
}

void Map::draw(sf::RenderTarget &window)
{
	for (auto &layer : m_layers)
		drawLayer(window, layer.get());
}

void Map::drawLayer(sf::RenderTarget& window, Layer* layer)
{
	layer->process(m_clock);

	if (layer->visible)
		layer->draw(window);
}

void Map::splitDraw(sf::RenderTarget &window, const std::string& byLayer, DrawType drawType)
{
	bool isDrawing = drawType == DrawType::Back;

	for (auto &layer : m_layers)
	{
		if (layer->name == byLayer)
		{
			if (isDrawing)
				break;
			
			isDrawing = true;
			continue;
		}

		if (isDrawing)
			drawLayer(window, layer.get());
	}
}

TileLayer *Map::getTileLayer(const std::string& layerName)
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

void Map::pause()
{
	m_clock.pause();
}

void Map::resume()
{
	m_clock.resume();
}

void Map::loadTileSets(const Json::Value &root, TextureMap &textures) // Loads all the images used by the json file as textures
{
	for (auto &val : root["tilesets"])
	{
		auto image = m_currentPath / val["image"].asString();
		int firstgid = val["firstgid"].asInt();
		auto it = textures.find(image.string());

		if (it == textures.end())
		{
			auto &&[it, inserted] = textures.try_emplace(image.string(), sf::Texture());
			if (inserted)
			{
				it->second.loadFromFile(image.string());
				tileSets.try_emplace(firstgid, &it->second);
			}
		}
		else
			tileSets.try_emplace(firstgid, &it->second);
		
		loadAnimatedTiles(firstgid, val["tiles"]);
	}
}

void Map::loadAnimatedTiles(int firstGid, const Json::Value &tileset) // Store info on animated tiles
{
	for (auto it = tileset.begin(); it != tileset.end(); ++it)
	{
		std::string tileid = it.key().asString();
		std::vector<std::pair<int, int>> tileSetAnimations;

		for (const auto &animation : tileset[tileid]["animation"])
		{
			int animationTileId = animation["tileid"].asInt();
			int animationTileDuration = animation["duration"].asInt();
			
			tileSetAnimations.emplace_back(animationTileId, animationTileDuration);
		}

		animatedTiles.try_emplace(firstGid + std::atoi(tileid.c_str()), tileSetAnimations);
	}
}
