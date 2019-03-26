#pragma once

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;
	
	virtual sf::FloatRect getLocalBounds() const;
	virtual sf::Transform getTransform() const;
	sf::FloatRect getGlobalBounds() const;

	bool intersects(const GameObject &object) const;
	bool contains(double x, double y) const;

	virtual sf::Vector2<double> getPosition() const { return sf::Vector2<double>(); }
	virtual void setPosition(double x, double y) {}
	virtual void setPosition(sf::Vector2<double> pos) {}

	virtual void draw(sf::RenderTarget &target) {}

#ifdef _DEBUG 
	void drawDebugOutline(sf::RenderTarget &target) const;
#endif // _DEBUG
};
