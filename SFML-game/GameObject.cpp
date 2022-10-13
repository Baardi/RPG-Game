#include "stdafx.h"
#include "GameObject.hpp"
#include "libs/SFML/SFML/Collision.hpp"

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

	return sf::Collision::boundingBoxTest(transf1, bounds1, transf2, bounds2);
}

bool GameObject::contains(sf::Vector2f pos) const
{
	auto bounds1 = this->getLocalBounds();
	auto bounds2 = sf::FloatRect();
	
	auto transf1 = this->getTransform();
	auto transf2 = sf::Transform().translate(pos);

	return sf::Collision::boundingBoxTest(transf1, bounds1, transf2, bounds2);
}

sf::Vector2f GameObject::getBottomRightPosition() const
{
	auto pos = getPosition();
	auto bounds = getGlobalBounds();

	return pos + bounds.getSize();
}

#ifdef _DEBUG 
void GameObject::drawDebugOutline(sf::RenderTarget &target) const
{
	// Draw white outline for transformed rec (May be rotated)
	const sf::FloatRect bounds{ getLocalBounds() };
	sf::RectangleShape rec{ bounds.getSize() };

	rec.setFillColor(sf::Color::Transparent);
	rec.setOutlineColor(sf::Color::White);
	rec.setOutlineThickness(-1.5);

	sf::RenderStates renderstate;
	renderstate.transform *= getTransform();
	target.draw(rec, renderstate);

	// Draw red outline for global bounds (unrotated)
	const sf::FloatRect globals{ getGlobalBounds() };
	sf::RectangleShape rec2{ globals.getSize() };

	rec2.setFillColor(sf::Color::Transparent);
	rec2.setOutlineColor(sf::Color::Red);
	rec2.setOutlineThickness(-1.5);
	rec2.setPosition(globals.getPosition());
	
	target.draw(rec2);
}
#endif // _DEBUG
