#pragma once
#include <SFML/Graphics/Rect.hpp>

class GameObject
{
	friend class Map;
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	virtual sf::FloatRect GetGlobalBounds();
	bool Intersects(GameObject &object);

	//possibly implement later, if decided to be useful
	//virtual void setPosition();
	//virtual void draw();
};

