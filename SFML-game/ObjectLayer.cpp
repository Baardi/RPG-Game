#include "stdafx.h"
#include "ObjectLayer.h"
#include "map.h"

ObjectLayer::~ObjectLayer()
{
	for (auto object : objects)
		delete object;
}

void ObjectLayer::load(Json::Value& layer, sftools::Chronometer &clock)
{
	name = layer["name"].asString();
	type = layer["type"].asString();
	visible = layer["visible"].asBool();
	opacity = layer["opacity"].asFloat();
	LoadProperties(layer);

	// Get all mapObjects from layer
	for (Json::Value& object : layer["objects"])
	{
		ObjectSprite *sprite = new ObjectSprite(tileSize, tileSets, animatedTiles, clock);
		sprite->load(layer, object);
		objects.emplace_back(sprite);
	}
}

void ObjectLayer::process()
{
	for (auto &object : objects)
		object->process();
}

void ObjectLayer::draw(sf::RenderWindow& window)
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
	auto it = std::find(objects.begin(), objects.end(), sprite);
	if (it == objects.end())
		throw;
	
	objects.erase(it);
}

ObjectSprite *ObjectLayer::GetIntersectedObject(const GameObject& other)
{
	for (auto object : objects)
	{
		if (object->Intersects(other))
			return object;
	}

	return nullptr;
}
