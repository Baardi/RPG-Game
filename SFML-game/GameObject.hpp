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
	bool contains(sf::Vector2f offset) const;

	sf::Vector2f getBottomRightPosition() const;
	virtual sf::Vector2f getPosition() const { return sf::Vector2f(); }
	virtual void setPosition(float x, float y) {}
	virtual void setPosition(sf::Vector2f pos) {}

	virtual void draw(sf::RenderTarget &target) {}

#ifdef _DEBUG 
	void drawDebugOutline(sf::RenderTarget &target) const;
#endif // _DEBUG
};
