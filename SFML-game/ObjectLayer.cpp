#include "stdafx.h"
#include "ObjectLayer.hpp"
#include "Map.hpp"
#include <ranges>

bool ObjectLayer::load(const Json::Value& layer, const std::unordered_map<int, TileSet> &tileSets, const ObjectSpriteFactory &spriteFactory)
{
	name = layer["name"].asString();
	type = layer["type"].asString();
	visible = layer["visible"].asBool();
	opacity = layer["opacity"].asFloat();
	id = layer["id"].asInt();
	loadProperties(layer["properties"]);

	// Get all mapObjects from layer
	objects.reserve(layer["objects"].size());
	for (const auto &object : layer["objects"])
	{
		auto sprite = spriteFactory.create(object["type"].asString());
		sprite->load(layer, object, tileSets);
		objects.push_back(std::move(sprite));
	}

	return true;
}

bool ObjectLayer::save(Json::Value &layers) const
{
	Json::Value layer;

	layer["type"] = "objectgroup";
	layer["draworder"] = "topdown";
	layer["id"] = id;
	layer["name"] = name;
	layer["opacity"] = opacity;
	layer["visible"] = visible;
	layer["x"] = 0;
	layer["y"] = 0;

	saveProperties(layer["properties"]);

	for (const auto &object : objects)
		object->save(layer["objects"]);

	layers.append(layer);
	
	return true;
}

void ObjectLayer::process(const sftools::Chronometer &clock)
{
	for (auto &object : objects)
		object->process(clock);
}

void ObjectLayer::draw(sf::RenderTarget &target)
{
	for (auto &object : objects)
	{
		if (object->visible)
			object->draw(target);
	}
}

void ObjectLayer::loadTexture(const std::unordered_map<int, TileSet> &tileSets)
{
	for (auto &object : objects)
		object->loadTexture(tileSets);
}

void ObjectLayer::removeSprite(ObjectSprite *sprite)
{
	auto it = std::ranges::find_if(objects, [sprite](const auto &object) { return object.get() == sprite;});

	if (it == objects.end())
		return;
	
	objects.erase(it);
}

ObjectSprite *ObjectLayer::getIntersectedObject(const GameObject &other)
{
	auto it = std::ranges::find_if(objects, [&other](const auto &object) { return object->intersects(other); });

	if (it == objects.end())
		return nullptr;
	
	return &**it;
}
