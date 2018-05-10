#include "stdafx.h"
#include "SnakeHead.h"


SnakeHead::SnakeHead(sf::RenderWindow &window, sf::Color color) : window(window)
{
	shape.setPosition(xPos, yPos);
	shape.setFillColor(color);
	shape.setRadius(20);
}


SnakeHead::~SnakeHead()
{
}

void SnakeHead::draw()
{
	window.draw(shape);
}

sf::FloatRect SnakeHead::GetGlobalBounds()
{
	return shape.getGlobalBounds();
}

void SnakeHead::move(double xVector, double yVector, double speed)
{
	xPos = xPos + xVector * speed;
	yPos = yPos + yVector * speed;

	if (xPos < -10)
		xPos += 960;
	else if (xPos > 960-10)
		xPos -= 960;

	if (yPos < -10)
		yPos += 960;
	else if (yPos > 960-10)
		yPos -= 960;

	shape.setPosition(xPos, yPos);
}

void SnakeHead::setPosition(double xPos, double yPos)
{
	shape.setPosition(xPos, yPos);
}
