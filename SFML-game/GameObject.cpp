#include "stdafx.h"
#include "GameObject.hpp"

sf::FloatRect GameObject::getGlobalBounds() const
{
	return sf::FloatRect();
}

bool GameObject::intersects(const GameObject &object) const
{
	auto bounds1 = this->getGlobalBounds();
	auto bounds2 = object.getGlobalBounds();
	return bounds1.intersects(bounds2);
}

bool GameObject::contains(double x, double y) const
{
	auto bounds = getGlobalBounds();
	return bounds.contains(x, y);
}
