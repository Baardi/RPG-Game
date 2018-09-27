#include "stdafx.h"
#include <fstream>
#include "map.h"
#include <filesystem>
#include "utility.h"
#include "State.h"

#define _USE_MATH_DEFINES
#include "math.h"
#undef _USE_MATH_DEFINES

Map::~Map()
{
	clear();
}

void Map::clear()
{
	for (auto object : layers)
		delete object;
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
	
	currentPath = canonical(filename).parent_path();

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
		if (layer["type"].asString() == "tilelayer")
			loadLayer(layer);
		
		if (layer["type"].asString() == "objectgroup")
			loadObjects(layer);
	}

	return true;
}

bool Map::loadRelative(const std::filesystem::path& filename, TextureMap &textures)
{
	return load(canonical(currentPath / filename), textures);
}

std::filesystem::path Map::GetPathProperty(const std::string &propertyName)
{
	return canonical(currentPath / GetProperty<std::filesystem::path>(propertyName));
}

void Map::loadLayer(Json::Value& layer)
{
	Json::Value &data = layer["data"];

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
	tmp->LoadProperties(layer);
	layers.push_back(tmp);				   // vector, so the order is kept
	tileMap.try_emplace(tmp->name, tmp);   // so the layer can be retrieved later (e.g by game-class)
}

void Map::loadObjects(Json::Value& layer)
{
	ObjectLayer *objectLayer = new ObjectLayer(tileSize, tileSets, animatedTiles);
	objectLayer->name = layer["name"].asString();
	objectLayer->type = layer["type"].asString();
	objectLayer->visible = layer["visible"].asBool();
	objectLayer->opacity = layer["opacity"].asFloat();
	objectLayer->LoadProperties(layer);

	// Get all mapObjects from layer
	for (Json::Value& object: layer["objects"])
	{
		ObjectSprite* sprite = new ObjectSprite(tileSize, tileSets, animatedTiles, clock);

		// Load basic object info
		sprite->name = object["name"].asString();
		
		sprite->width = object["width"].asFloat();
		sprite->height = object["height"].asFloat();
		sprite->rotation = object["rotation"].asFloat();
		sprite->type = object["type"].asString();
		sprite->visible = object["visible"].asBool();
		sprite->opacity = layer["opacity"].asFloat();

		unsigned int gid = object["gid"].asUInt();
		sprite->verflip = gid / (flipMultiplier * 2);
		sprite->horflip = gid % (flipMultiplier * 2) > flipMultiplier;
		sprite->gid = gid % flipMultiplier;

		sprite->x = object["x"].asFloat();
		sprite->y = object["y"].asFloat();

		if (gid)
		{
			sprite->x += sprite->height * sin(sprite->rotation * (M_PI / 180.0));
			sprite->y -= sprite->height * cos(sprite->rotation * (M_PI / 180.0));
		}
		
		sprite->globalBounds = sf::DoubleRect(sprite->x, sprite->y, sprite->width, sprite->height);

		auto textValue = object["text"];
		if (!textValue.empty())
		{
			sprite->text = std::make_unique<sf::Text>();
			auto &text = *sprite->text.get();

			text.setFillColor(sf::utility::parseColor(textValue["color"].asString()));
			text.setString(textValue["text"].asString());
			text.setCharacterSize(16);
			text.setFont(State::Font());
			auto text_x = object["x"].asFloat();
			auto text_y = object["y"].asFloat();
			text.setPosition(text_x, text_y);
			text.setRotation(sprite->rotation);
		}

		sprite->LoadProperties(object);

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
			sf::Texture *tileSet = new sf::Texture();
			tileSet->loadFromFile(image.string());
			textures.try_emplace(image.string(), tileSet);
			tileSets.try_emplace(firstgid, tileSet);
		}
		else
			tileSets.try_emplace(firstgid, it->second);
		
		loadAnimatedTiles(firstgid, val["tiles"]);
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

		animatedTiles.try_emplace(firstGid + std::atoi(tileid.c_str()), tileSetAnimations);
	}
}
