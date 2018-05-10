#include "stdafx.h"
#include <fstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include <json/json.h>

Map::~Map()
{
	for (auto tileSet : tileSets)
		delete tileSet.second;
}

bool Map::load(std::string filename, std::vector<MapObject*>& objects)
{
	for (auto tileSet : tileSets)
		delete tileSet.second;

	tileSets.clear();

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
	TileSize tileSize;
	tileSize.x = root["tilewidth"].asInt();
	tileSize.y = root["tileheight"].asInt();
	tileSize.s = 0;

	loadTileSets(root);

	// Read in each layer
	for (Json::Value& layer: root["layers"])
	{
		loadLayer(layer, objects, tileSize);
		loadObjects(root, layer, objects, tileSize);
	}

	return true;
}

void Map::loadLayer(Json::Value& layer, std::vector<MapObject*>& objects, TileSize tileSize)
{
	Layer *tmp = new Layer(tileSize, tileSets, animatedTiles); // Layer needs a reference to the active tilesets

	// Store info on layer
	tmp->width = layer["width"].asInt();
	tmp->height = layer["height"].asInt();
    tmp->name = layer["name"].asString();

	// Clear tilemap
	memset(tmp->tilemap, 0, sizeof(tmp->tilemap));

	// Read in tilemap
	Json::Value &data = layer["data"];
	for (unsigned int i = 0; i < data.size(); i++)
	{
		tmp->tilemap[i % tmp->width][i / tmp->width] = data[i].asInt();
	}

	if (tmp->name=="powerups")
		powerup_layer = tmp;
    else if (tmp->name=="barrels")
		box_layer = tmp;
    else if (tmp->name=="indestructable")
		brick_layer = tmp;

	tmp->loadTexture();
	objects.push_back(tmp);
}

void Map::loadObjects(Json::Value& root, Json::Value& layer, std::vector<MapObject*>& objects, TileSize tileSize)
{
	// Get all mapObjects from layer
	for (Json::Value& object: layer["objects"])
	{
		Sprite* sprite = new Sprite(tileSize, tileSets, animatedTiles);

		// Load basic object info
		sprite->x = object["x"].asInt();
		sprite->y = object["y"].asInt() - sprite->tileSize.y;
		sprite->id = object["gid"].asInt();

		sprite->loadTexture();
		objects.push_back(sprite);
	}
}

void Map::clear_arrays()
{
	for(int i = 0; i<15;i++)
	{
		for(int j = 0; j<15;j++)
		{
			bombs[i][j]=0;
			explosion[i][j]=0;
		}
	}
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
			
			tileSetAnimations.push_back(std::make_pair(animationTileId, animationTileDuration));
		}
		animatedTiles.try_emplace(firstGid + atoi(tileid.c_str()), tileSetAnimations);
	}
}

void Map::loadAnimatedObjects(int firstGid, Json::Value &tileset) // Store info on animated tiles
{
	for (Json::ValueIterator it = tileset.begin(); it != tileset.end(); ++it)
	{
		std::string tileid = it.key().asString();
		std::vector<std::pair<int, int>> tileSetAnimations;

		for (Json::Value &animation : tileset[tileid]["animation"])
		{
			int animationTileId = animation["tileid"].asInt();
			int animationTileDuration = animation["duration"].asInt();

			tileSetAnimations.push_back(std::make_pair(animationTileId, animationTileDuration));
		}
		animatedTiles.try_emplace(firstGid + atoi(tileid.c_str()), tileSetAnimations);
	}
}