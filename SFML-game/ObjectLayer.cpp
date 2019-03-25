#include "stdafx.h"
#include "ObjectLayer.hpp"
#include "map.hpp"

ObjectLayer::~ObjectLayer()
{
}

void ObjectLayer::load(const Json::Value& layer, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
{
	name = layer["name"].asString();
	type = layer["type"].asString();
	visible = layer["visible"].asBool();
	opacity = layer["opacity"].asFloat();
	loadProperties(layer);

	// Get all mapObjects from layer
	for (const auto &object : layer["objects"])
	{
		auto &sprite = objects.emplace_back(
			std::make_unique<ObjectSprite>(tileSize));

		sprite->load(layer, object, tileSets, animatedTiles);
	}
}

void ObjectLayer::process(const sftools::Chronometer &clock)
{
	for (auto &object : objects)
		object->process(clock);
}

void ObjectLayer::draw(sf::RenderTarget& window)
{
	for (auto &object : objects)
	{
		if (object->visible)
			object->draw(window);
	}
}

void ObjectLayer::loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
{
	for (auto &object : objects)
		object->loadTexture(tileSets, animatedTiles);
}

void ObjectLayer::removeSprite(ObjectSprite* sprite)
{
	auto it = std::find_if(objects.begin(), objects.end(), 
		[sprite](auto &object) { return object.get() == sprite;});

	if (it == objects.end())
		throw;
	
	objects.erase(it);
}

ObjectSprite *ObjectLayer::getIntersectedObject(const GameObject& other)
{
	for (auto &object : objects)
	{
		if (object->intersects(other))
			return object.get();
	}

	return nullptr;
}
