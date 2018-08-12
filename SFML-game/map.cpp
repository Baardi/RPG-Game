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

	for (auto object : layers)
		delete object;
	
	if (walkables)
		delete[] walkables;

	tileSets.clear();
	layers.clear();
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
	width = root["width"].asInt();
	height = root["height"].asInt();

	loadTileSets(root);

	// Read in each layer
	for (Json::Value& layer: root["layers"])
	{
		if (layer["type"].asString() == "tilelayer")
			loadLayer(layer);
		
		if (layer["type"].asString() == "objectgroup")
			loadObjects(layer);
	}
	SetWalkables();

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
	layers.push_back(tmp);				   // vector, so the order is kept
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

	layers.push_back(objectLayer);
	objectMap.try_emplace(objectLayer->name, objectLayer);
}

void Map::draw(sf::RenderWindow &window)
{
	for (auto layer : layers)
		drawLayer(window, layer);
}

void Map::drawLayer(sf::RenderWindow& window, Layer* layer)
{
	layer->process();

	if (layer->visible)
		layer->draw(window);
}

void Map::splitDraw(sf::RenderWindow &window, const std::string& byLayer, DrawType drawType)
{
	bool isDrawing = drawType == DrawType::Back ? true : false;

	for (auto layer : layers)
	{
		if (layer->name == byLayer)
		{
			if (isDrawing)
				break;
			
			isDrawing = true;
			continue;
		}

		if (isDrawing)
			drawLayer(window, layer);
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

// Assumes tilesize is similar for the entire game

bool Map::isWalkableTileCoords(int xPos, int yPos)
{
	return get(walkables, xPos, yPos) 
	&& get(walkables, xPos, yPos + 1)
	&& get(walkables, xPos + 1, yPos)
	&& get(walkables, xPos + 1, yPos + 1);
}

// Need conversion method or so to account for spacing
bool Map::isWalkableScreenCoords(int xCoord, int yCoord)
{
	return get(walkables, xCoord / tileSize.x, yCoord / tileSize.y)
	&& get(walkables, 1 + xCoord / tileSize.x, yCoord / tileSize.y)
	&& get(walkables, xCoord / tileSize.x, 1 + yCoord / tileSize.y)
	&& get(walkables, 1 + xCoord / tileSize.x, 1 + yCoord / tileSize.y);
}

void Map::SetWalkables()
{
	walkables = new bool[width*height];
	auto unwalkables = GetTileLayer("Unwalkables");

	for (int x = 0; x < width; x++)
		for (int y = 0;y < height; y++)
			get(walkables, x, y) = !unwalkables->containsTexture(x, y);
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
