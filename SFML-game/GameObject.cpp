#include "stdafx.h"
#include "GameObject.hpp"
#include "SFML/Collision.hpp"


sf::FloatRect GameObject::getLocalBounds() const
{
	return sf::FloatRect();
}

sf::Transform GameObject::getTransform() const
{
	return sf::Transform();
}

sf::FloatRect GameObject::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

bool GameObject::intersects(const GameObject &object) const
{
	auto bounds1 = this->getLocalBounds();
	auto bounds2 = object.getLocalBounds();

	auto transf1 = this->getTransform();
	auto transf2 = object.getTransform();

	return sf::Collision::BoundingBoxTest(transf1, bounds1, transf2, bounds2);
}

bool GameObject::contains(double x, double y) const
{
	auto bounds1 = this->getLocalBounds();
	auto bounds2 = sf::FloatRect();
	
	auto transf1 = this->getTransform();
	auto transf2 = sf::Transform().translate(x, y);

	return sf::Collision::BoundingBoxTest(transf1, bounds1, transf2, bounds2);
}
