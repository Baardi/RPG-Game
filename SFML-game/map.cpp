#include "stdafx.h"
#include <fstream>
#include <SFML/Graphics.hpp>
#include "map.h"

#define _USE_MATH_DEFINES
#include "math.h"
#undef _USE_MATH_DEFINES

Map::~Map()
{
	clear();
}

void Map::clear()
{
	for (auto tileSet : tileSets)
		delete tileSet.second;

	for (auto object : objects)
		delete object;

	tileSets.clear();
	objects.clear();
	objectMap.clear();
	tileMap.clear();
	animatedTiles.clear();
	clock.restart();
}

bool Map::load(const std::string &filename)
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

	// Get tile size information
	tileSize.x = root["tilewidth"].asInt();
	tileSize.y = root["tileheight"].asInt();
	tileSize.s = root["spacing"].asInt();

	loadTileSets(root);

	// Read in each layer
	for (Json::Value& layer: root["layers"])
	{
		if (layer["type"].asString() == "tilelayer")
			loadLayer(layer);
		
		if (layer["type"].asString() == "objectgroup")
			loadObjects(layer);
	}

	return true;
}

void Map::loadLayer(Json::Value& layer)
{
	Json::Value &data = layer["data"];

	if (data.empty()) // probably an object layer
		return;

	TileLayer *tmp = new TileLayer(tileSize, tileSets, animatedTiles, clock); // TileLayer needs a reference to the active tilesets

	// Store info on layer
	tmp->width = layer["width"].asInt();
	tmp->height = layer["height"].asInt();
	tmp->name = layer["name"].asString();
	tmp->visible = layer["visible"].asBool();
	tmp->opacity = layer["opacity"].asFloat();

	// Prepare tilemap
	tmp->initArrays();
	memset(tmp->tilemap, 0, sizeof(tmp->tilemap));

	// Read in tilemap
	for (unsigned int i = 0; i < data.size(); i++)
	{
		tmp->tilemap[i] = data[i].asInt();
	}

	tmp->loadTexture();
	objects.push_back(tmp);				   // vector, so the order is kept
	tileMap.try_emplace(tmp->name, tmp); // so the layer can be retrieved later (e.g by game-class)
}

void Map::loadObjects(Json::Value& layer)
{
	ObjectLayer *objectLayer = new ObjectLayer(tileSize, tileSets, animatedTiles);
	objectLayer->name = layer["name"].asString();
	objectLayer->type = layer["type"].asString();
	objectLayer->visible = layer["visible"].asBool();
	objectLayer->opacity = layer["opacity"].asFloat();

	// Get all mapObjects from layer
	for (Json::Value& object: layer["objects"])
	{
		ObjectSprite* sprite = new ObjectSprite(tileSize, tileSets, animatedTiles, clock);

		// Load basic object info
		sprite->name = object["name"].asString();
		
		unsigned int _gid = object["gid"].asUInt();
		sprite->verflip = _gid / (flipMultiplier * 2);
		sprite->horflip = _gid % (flipMultiplier * 2) > flipMultiplier;
		sprite->gid = _gid % flipMultiplier;

		sprite->width = object["width"].asFloat();
		sprite->height = object["height"].asFloat();
		sprite->rotation = object["rotation"].asFloat();
		sprite->x = object["x"].asFloat() + sprite->height * sin(sprite->rotation * (M_PI / 180.0));
		sprite->y = object["y"].asFloat() - sprite->height * cos(sprite->rotation * (M_PI / 180.0));
		sprite->type = object["type"].asString();
		sprite->visible = object["visible"].asBool();
		sprite->opacity = layer["opacity"].asFloat();
		
		auto &propertytypes = object["propertytypes"];
		auto &properties = object["properties"];
		for (Json::ValueIterator it = properties.begin(); it != properties.end(); ++it)
		{
			if (propertytypes[it.key().asString()] == "string")
				sprite->propertyMap.try_emplace(it.key().asString(), it->asString());
		}

		sprite->loadTexture();
		objectLayer->objects.emplace_back(sprite);
	}

	objects.push_back(objectLayer);
	objectMap.try_emplace(objectLayer->name, objectLayer);
}

void Map::draw(sf::RenderWindow &window)
{
	for (auto object : objects)
		object->process();

	for (auto object : objects)
	{
		if (object->visible)
			object->draw(window);
	}
}

TileLayer *Map::GetTileLayer(const std::string& layerName)
{
	return tileMap[layerName];
}

ObjectLayer *Map::GetObjectLayer(const std::string& layerName)
{
	return objectMap[layerName];
}

void Map::loadTileSets(Json::Value &root) // Loads all the images used by the json file as textures
{
	for (auto &val : root["tilesets"])
	{
		sf::Texture *tileSet = new sf::Texture();
		tileSet->loadFromFile("data/" + val["image"].asString());
		tileSets.try_emplace(val["firstgid"].asInt(), tileSet);

		loadAnimatedTiles(val["firstgid"].asInt(), val["tiles"]);
	}
}

void Map::loadAnimatedTiles(int firstGid, Json::Value &tileset) // Store info on animated tiles
{
	for (Json::ValueIterator it = tileset.begin(); it != tileset.end(); ++it)
	{
		std::string tileid = it.key().asString();
		std::vector<std::pair<int, int>> tileSetAnimations;

		for (Json::Value &animation : tileset[tileid]["animation"])
		{
			int animationTileId = animation["tileid"].asInt();
			int animationTileDuration = animation["duration"].asInt();
			
			tileSetAnimations.emplace_back(std::make_pair(animationTileId, animationTileDuration));
		}

		animatedTiles.try_emplace(firstGid + atoi(tileid.c_str()), tileSetAnimations);
	}
}
