#include "stdafx.h"
#include "IntersectionHandler.h"
#include "ObjectSprite.h"
#include "ObjectLayer.h"

void IntersectionHandler::HandleIntersections(const Map &map, GameObject &object)
{
	for (auto &entry : map.objectMap)
	{
		auto layer = entry.second;

		auto it = intersectionMap.find(layer->name);
		if (it == intersectionMap.end())
			continue;

		auto sprite = layer->GetIntersectedObject(object);
		if (!sprite)
			continue;
		
		it->second(layer, sprite);
		break; // Calling the function can potentially break the iteration
	}
}

void IntersectionHandler::Register(const std::string &name, std::function<void(ObjectLayer *, ObjectSprite *)> action)
{
	// Be careful, making changes to the map, might break the layer
	// Making changes to to the layer, might break the object
	intersectionMap.try_emplace(name, action);
}
