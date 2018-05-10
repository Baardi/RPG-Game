#include "stdafx.h"
#include "SnakeBodyElem.h"


SnakeBodyElem::SnakeBodyElem(sf::RenderWindow &window, sf::Color color, double xPos, double yPos) : xPos(xPos), yPos(yPos), window(window)
{
	this->xPos = xPos;
	this->yPos = yPos;

	//should be init function <-- orly?
	shape.setRadius(20);
	shape.setFillColor(color);
	shape.setPosition(xPos, yPos);
}


SnakeBodyElem::~SnakeBodyElem()
{
}

void SnakeBodyElem::draw()
{
	window.draw(shape);
}

void SnakeBodyElem::setPosition(double xPos, double yPos)
{
	this->xPos = xPos;
	this->yPos = yPos;
	shape.setPosition(xPos,yPos);
}

sf::FloatRect SnakeBodyElem::GetGlobalBounds()
{
	return shape.getGlobalBounds();
}
