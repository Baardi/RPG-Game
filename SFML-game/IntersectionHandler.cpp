#include "stdafx.h"
#include "IntersectionHandler.h"
#include "ObjectSprite.h"
#include "ObjectLayer.h"
#include "map.h"

IntersectionHandler::IntersectionHandler()
{
}


IntersectionHandler::~IntersectionHandler()
{
}

ObjectSprite* IntersectionHandler::GetIntersectedObject(const ObjectLayer *layer, const GameObject &intersected)
{
	ObjectSprite *retObject = nullptr;
	for (auto object : layer->objects)
	{
		if (object->Intersects(intersected))
		{
			retObject = object;
			break;
		}
	}
	return retObject;
}

void IntersectionHandler::HandleIntersections(const Map &map, GameObject &object)
{
	for (auto layer : map.layers)
	{
		auto objectLayer = dynamic_cast<ObjectLayer *>(layer);
		if (!objectLayer)
			continue;

		auto it = intersectionMap.find(objectLayer->name);
		if (it == intersectionMap.end())
			continue;

		auto sprite = GetIntersectedObject(objectLayer, object);
		if (!sprite)
			continue;
				
		it->second(objectLayer, sprite);
	}
}

void IntersectionHandler::Register(const std::string &name, std::function<void(ObjectLayer *, ObjectSprite *)> action)
{
	intersectionMap.try_emplace(name, action);
}
