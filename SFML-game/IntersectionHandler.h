#pragma once
#include <functional>
#include "ObjectSprite.h"
#include "map.h"

class IntersectionHandler
{
public:
	IntersectionHandler() = default;
	~IntersectionHandler() = default;

	void handleEvents(const Map &map, GameObject &object);
	void registerEvent(const std::string &name, const std::function<void(ObjectLayer *, ObjectSprite *)> &action);

private:
	std::map<std::string, std::function<void(ObjectLayer *, ObjectSprite *)>> intersectionMap;
};

