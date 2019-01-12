#include "stdafx.h"
#include "ObjectLayer.h"
#include "map.h"

ObjectLayer::~ObjectLayer()
{
}

void ObjectLayer::load(const Json::Value& layer, sftools::Chronometer &clock)
{
	name = layer["name"].asString();
	type = layer["type"].asString();
	visible = layer["visible"].asBool();
	opacity = layer["opacity"].asFloat();
	LoadProperties(layer);

	// Get all mapObjects from layer
	for (const auto &object : layer["objects"])
	{
		auto &sprite = objects.emplace_back(
			std::make_unique<ObjectSprite>(tileSize, tileSets, animatedTiles, clock));

		sprite->load(layer, object);
	}
}

void ObjectLayer::process()
{
	for (auto &object : objects)
		object->process();
}

void ObjectLayer::draw(sf::RenderTarget& window)
{
	for (auto &object : objects)
	{
		if (object->visible)
			object->draw(window);
	}
}

void ObjectLayer::loadTexture()
{
	for (auto &object : objects)
		object->loadTexture();
}

void ObjectLayer::RemoveSprite(ObjectSprite* sprite)
{
	auto it = std::find_if(objects.begin(), objects.end(), 
		[sprite](auto &object) { return object.get() == sprite;});

	if (it == objects.end())
		throw;
	
	objects.erase(it);
}

ObjectSprite *ObjectLayer::GetIntersectedObject(const GameObject& other)
{
	for (auto &object : objects)
	{
		if (object->Intersects(other))
			return object.get();
	}

	return nullptr;
}
