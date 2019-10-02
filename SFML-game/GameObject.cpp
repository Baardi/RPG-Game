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

sf::Vector2f GameObject::getBottomRightPosition() const
{
	auto pos = getPosition();
	auto bounds = getGlobalBounds();

	return sf::Vector2f(pos.x + bounds.width, pos.y + bounds.height);
}

#ifdef _DEBUG 
void GameObject::drawDebugOutline(sf::RenderTarget &target) const
{
	// Draw white outline for transformed rec (May be rotated)
	sf::FloatRect bounds = getLocalBounds();
	sf::RectangleShape rec(sf::Vector2f(bounds.width, bounds.height));

	rec.setFillColor(sf::Color::Transparent);
	rec.setOutlineColor(sf::Color::White);
	rec.setOutlineThickness(-1.5);

	sf::RenderStates renderstate;
	renderstate.transform *= getTransform();
	target.draw(rec, renderstate);

	// Draw red outline for global bounds (unrotated)
	sf::FloatRect globals = getGlobalBounds();
	sf::RectangleShape rec2(sf::Vector2f(globals.width, globals.height));

	rec2.setFillColor(sf::Color::Transparent);
	rec2.setOutlineColor(sf::Color::Red);
	rec2.setOutlineThickness(-1.5);
	rec2.setPosition(globals.left, globals.top);
	
	target.draw(rec2);
}
#endif // _DEBUG
