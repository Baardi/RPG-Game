#include "stdafx.h"
#include "GameObject.h"

sf::FloatRect GameObject::GetGlobalBounds()
{
	return sf::FloatRect();
}

bool GameObject::Intersects(GameObject &object)
{
	auto bounds1 = this->GetGlobalBounds();
	auto bounds2 = object.GetGlobalBounds();
	return bounds1.intersects(bounds2);
}