#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class SnakeHead : public GameObject
{
public:
	SnakeHead(sf::RenderWindow &window, sf::Color color);
	~SnakeHead();
	sf::FloatRect GetGlobalBounds() override;

	void setPosition(double xPos, double yPos);
	void move(double xVector, double yVector, double speed);
	void draw();
	
	double xPos = 490;
	double yPos = 490;

private:
	sf::RenderWindow &window;
	sf::CircleShape shape;
};



