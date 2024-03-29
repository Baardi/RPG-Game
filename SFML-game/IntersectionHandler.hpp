#pragma once
#include <functional>
#include "ObjectSprite.hpp"
#include "Map.hpp"

class IntersectionHandler
{
public:
	IntersectionHandler() = default;
	~IntersectionHandler() = default;

	void handleEvents(const Map &map, GameObject &object);
	void registerEvent(const std::string &name, const std::function<void(ObjectLayer *, ObjectSprite *)> &action);

private:
	std::unordered_map<std::string, std::function<void(ObjectLayer *, ObjectSprite *)>> m_intersectionMap;
};

