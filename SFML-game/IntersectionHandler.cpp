#include "stdafx.h"
#include "IntersectionHandler.hpp"
#include "ObjectSprite.hpp"
#include "ObjectLayer.hpp"

void IntersectionHandler::handleEvents(const Map &map, GameObject &object)
{
	for (const auto &[layerName, layer] : map.m_objectMap)
	{
		const auto it = intersectionMap.find(layerName);
		if (it == intersectionMap.end())
			continue;

		auto *sprite = layer->getIntersectedObject(object);
		if (!sprite)
			continue;
		
		it->second(layer, sprite);
		break; // Calling the function can potentially break the iteration
	}
}

void IntersectionHandler::registerEvent(const std::string &name, const std::function<void(ObjectLayer *, ObjectSprite *)> &action)
{
	// Be careful, making changes to the map, might break the layer
	// Making changes to to the layer, might break the object
	intersectionMap.try_emplace(name, action);
}
