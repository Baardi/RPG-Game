#pragma once
#include "GameObject.hpp"
#include <vector>

/* 
*	Helper class to enable intermediate drawing within a layer
*/
class FringeDrawer
{
public:
	/*
	*	Adds a reference to the object to the fringe-drawing collection
	*	The object will be drawn at the appropriate time
	*/
	void addObject(GameObject &object)
	{
		m_objects.emplace_back(&object);
	}

	/*
	*	Draws all stored objects with a bottom right coordinate between given y-minimum and y-maximum
	*/
	void drawBetween(sf::RenderTarget &target, float yMin, float yMax)
	{
		for (auto &object : m_objects)
		{
			auto objPos = object->getBottomRightPosition();
			if (objPos.y <= yMax && objPos.y > yMin)
				object->draw(target);
		}
	}

private:
	std::vector<GameObject *> m_objects;
};
