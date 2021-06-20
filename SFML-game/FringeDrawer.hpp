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
	void drawBetween(sf::RenderTarget &target, double yMin, double yMax) const
	{
		for (auto &object : m_objects)
		{
			auto objPos = object->getBottomRightPosition();
			if (objPos.y <= yMax && objPos.y > yMin)
				object->draw(target);
		}
	}

	/*
	*	Sorts objects before drawing a frame
	*/
	void sortObjects()
	{
		std::ranges::sort(m_objects, [] (const GameObject *left, const GameObject *right)
		{
			auto leftPos = left->getBottomRightPosition();
			auto rightPos = right->getBottomRightPosition();
			return leftPos.y < rightPos.y;
		});
	}

private:
	std::vector<GameObject *> m_objects;
};
