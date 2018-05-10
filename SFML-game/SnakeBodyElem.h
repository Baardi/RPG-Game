#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include "GameObject.h"
#include <SFML/Graphics/RenderWindow.hpp>

class SnakeBodyElem : public GameObject
{
public:
	SnakeBodyElem(sf::RenderWindow &window, sf::Color color, double xPos, double yPos);
	~SnakeBodyElem();
	sf::FloatRect GetGlobalBounds() override;
	void draw();
	void setPosition(double xPos, double yPos);

	double xPos;
	double yPos;

private:
	sf::RenderWindow &window;
	sf::CircleShape shape;
};

