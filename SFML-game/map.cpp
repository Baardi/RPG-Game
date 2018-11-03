#include "stdafx.h"
#include <fstream>
#include "map.h"
#include <filesystem>
#include "State.h"

Map::~Map()
{
}

void Map::clear()
{
	layers.clear();
	tileSets.clear();
	objectMap.clear();
	tileMap.clear();
	animatedTiles.clear();
	propertyMap.clear();
	currentPath = "";

	clock.reset(true);
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
		throw;

	currentPath = std::filesystem::canonical(filename).parent_path();

	// Get tile size information
	tileSize.x = root["tilewidth"].asInt();
	tileSize.y = root["tileheight"].asInt();
	tileSize.s = root["spacing"].asInt();
	width = root["width"].asInt();
	height = root["height"].asInt();

	loadTileSets(root, textures);

	LoadProperties(root);

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

bool Map::loadRelative(const std::filesystem::path& filename, TextureMap &textures)
{
	return load(std::filesystem::canonical(currentPath / filename), textures);
}

bool Map::GetPathProperty(const std::string &propertyName, std::filesystem::path &property) const
{
	if (!GetProperty<std::filesystem::path>(propertyName, property))
		return false;
		
	property = std::filesystem::canonical(currentPath / property);
}

void Map::loadLayer(Json::Value& layer)
{
	// Store info on layer
	auto tmp = static_cast<TileLayer *>(layers.emplace_back(
		std::make_unique<TileLayer>(tileSize, tileSets, animatedTiles, clock))
		.get());				   // vector, so the order is kept

	tmp->load(layer);
	tileMap.try_emplace(tmp->name, tmp);   // so the layer can be retrieved later (e.g by game-class)
}

void Map::loadObjects(Json::Value& layer)
{
	// Store info on layer
	auto objectLayer = static_cast<ObjectLayer *>(layers.emplace_back(
		std::make_unique<ObjectLayer>(tileSize, tileSets, animatedTiles))
		.get());				   // vector, so the order is kept
	
	objectLayer->load(layer, clock);
	objectMap.try_emplace(objectLayer->name, objectLayer); // so the layer can be retrieved later (e.g by game-class)
}

void Map::draw(sf::RenderTarget &window)
{
	for (auto &layer : layers)
		drawLayer(window, layer.get());
}

void Map::drawLayer(sf::RenderTarget& window, Layer* layer)
{
	layer->process();

	if (layer->visible)
		layer->draw(window);
}

void Map::splitDraw(sf::RenderTarget &window, const std::string& byLayer, DrawType drawType)
{
	bool isDrawing = drawType == DrawType::Back;

	for (auto &layer : layers)
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

TileLayer *Map::GetTileLayer(const std::string& layerName)
{
	auto it = tileMap.find(layerName);
	if (it != tileMap.end())
		return it->second;

	return nullptr;
}

ObjectLayer *Map::GetObjectLayer(const std::string& layerName)
{
	auto it = objectMap.find(layerName);
	if (it != objectMap.end())
		return it->second;

	return nullptr;
}

void Map::pause()
{
	clock.pause();
}

void Map::resume()
{
	clock.resume();
}

void Map::loadTileSets(Json::Value &root, TextureMap &textures) // Loads all the images used by the json file as textures
{
	for (auto &val : root["tilesets"])
	{
		auto image = currentPath / val["image"].asString();
		int firstgid = val["firstgid"].asInt();
		auto it = textures.find(image.string());

		if (it == textures.end())
		{
			auto inserted = textures.try_emplace(image.string(), sf::Texture());
			if (inserted.second) // Whether it succeeded
			{
				sf::Texture &tileSet = inserted.first->second;
				tileSet.loadFromFile(image.string());
				tileSets.try_emplace(firstgid, &tileSet);
			}
		}
		else
			tileSets.try_emplace(firstgid, &it->second);
		
		loadAnimatedTiles(firstgid, val["tiles"]);
	}
}

void Map::loadAnimatedTiles(int firstGid, Json::Value &tileset) // Store info on animated tiles
{
	for (auto it = tileset.begin(); it != tileset.end(); ++it)
	{
		std::string tileid = it.key().asString();
		std::vector<std::pair<int, int>> tileSetAnimations;

		for (Json::Value &animation : tileset[tileid]["animation"])
		{
			int animationTileId = animation["tileid"].asInt();
			int animationTileDuration = animation["duration"].asInt();
			
			tileSetAnimations.emplace_back(animationTileId, animationTileDuration);
		}

		animatedTiles.try_emplace(firstGid + std::atoi(tileid.c_str()), tileSetAnimations);
	}
}
