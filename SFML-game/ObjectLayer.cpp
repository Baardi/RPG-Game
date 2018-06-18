#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "ObjectLayer.h"

ObjectLayer::~ObjectLayer()
{
	for (auto object : objects)
		delete object;
}

void ObjectLayer::process()
{
	for (auto object : objects)
		object->process();
}

void ObjectLayer::draw(sf::RenderWindow& window)
{
	for (auto object : objects)
	{
		if (object->visible)
			object->draw(window);
	}
}

void ObjectLayer::loadTexture()
{
	for (auto object : objects)
		object->loadTexture();
}
