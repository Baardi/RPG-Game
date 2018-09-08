#include "stdafx.h"
#include "GameObject.h"

sf::DoubleRect GameObject::GetGlobalBounds()
{
	return sf::DoubleRect();
}

bool GameObject::Intersects(GameObject &object)
{
	auto bounds1 = this->GetGlobalBounds();
	auto bounds2 = object.GetGlobalBounds();
	return bounds1.intersects(bounds2);
}
