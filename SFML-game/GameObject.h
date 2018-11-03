#pragma once

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	virtual sf::DoubleRect GetGlobalBounds() const;
	bool Intersects(const GameObject &object) const;
	bool Contains(double x, double y) const;

	//possibly implement later, if decided to be useful
	virtual void SetPosition(double x, double y) {}
	virtual void draw(sf::RenderTarget &window) {}
};
