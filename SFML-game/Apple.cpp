#include "stdafx.h"
#include "Apple.h"


Apple::Apple(sf::RenderWindow &window) : window(window)
{
	 xPos = (std::rand() % 960-20);
	 yPos = (std::rand() % 960-20);
	shape.setPosition(xPos, yPos);
	shape.setFillColor(sf::Color::Red);
	shape.setRadius(20);
}


Apple::~Apple()
{
}

void Apple::draw()
{
	window.draw(shape);
}

void Apple::respawn()
{
	xPos = std::rand() % (960-20);
	yPos = std::rand() % (960-20);
	shape.setPosition(xPos, yPos);
}

sf::FloatRect Apple::GetGlobalBounds()
{
	return shape.getGlobalBounds();
}
