#pragma once
#include <functional>
#include "ObjectSprite.h"
#include "map.h"

class IntersectionHandler
{
public:
	IntersectionHandler() = default;
	~IntersectionHandler() = default;

	void HandleIntersections(const Map &map, GameObject &object);
	void Register(const std::string &name, std::function<void(ObjectLayer *, ObjectSprite *)> action);

private:
	std::map<std::string, std::function<void(ObjectLayer *, ObjectSprite *)>> intersectionMap;
};

