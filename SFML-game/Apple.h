#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Apple : public GameObject
{
public:
	Apple(sf::RenderWindow &window);
	~Apple();
	void respawn();
	void draw();

	sf::FloatRect GetGlobalBounds() override;

	int xPos;
	int yPos;

private:
	sf::CircleShape shape;
	sf::RenderWindow &window;
};



