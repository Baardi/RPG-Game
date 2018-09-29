#pragma once
#include <functional>
#include "ObjectSprite.h"

class IntersectionHandler
{
public:
	IntersectionHandler();
	~IntersectionHandler();

	static ObjectSprite* GetIntersectedObject(const ObjectLayer* layer, const GameObject &intersected);
	void HandleIntersections(const Map &map, GameObject &object);
	void Register(const std::string &name, std::function<void(ObjectLayer *, ObjectSprite *)> action);

private:
	std::map<std::string, std::function<void(ObjectLayer *, ObjectSprite *)>> intersectionMap;
};

