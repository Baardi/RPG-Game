#include "stdafx.h"
#include "GameObject.h"

sf::FloatRect GameObject::GetGlobalBounds() const
{
	return sf::FloatRect();
}

bool GameObject::Intersects(const GameObject &object) const
{
	auto bounds1 = this->GetGlobalBounds();
	auto bounds2 = object.GetGlobalBounds();
	return bounds1.intersects(bounds2);
}

bool GameObject::Contains(double x, double y) const
{
	auto bounds = GetGlobalBounds();
	return bounds.contains(x, y);
}
