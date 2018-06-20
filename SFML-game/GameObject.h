#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameObject
{
	friend class Map;
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	virtual sf::FloatRect GetGlobalBounds();
	bool Intersects(GameObject &object);

	//possibly implement later, if decided to be useful
	virtual void SetPosition(int x, int y) {}
	virtual void draw(sf::RenderWindow &window) {}
};

