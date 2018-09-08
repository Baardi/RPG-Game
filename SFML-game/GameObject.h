#pragma once

class GameObject
{
	friend class Map;
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	virtual sf::DoubleRect GetGlobalBounds();
	bool Intersects(GameObject &object);

	//possibly implement later, if decided to be useful
	virtual void SetPosition(double x, double y) {}
	virtual void draw(sf::RenderWindow &window) {}
};
