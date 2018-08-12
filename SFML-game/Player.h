#pragma once
#include <SFML/Graphics.hpp>
#include <sfml/chronometer.h>
#include "TileLayer.h"
#include "GameObject.h"

enum class Dir
{
	Down = 0,
	Up = 1,
	Left = 2,
	Right = 3
};

class Player : public GameObject
{
public:
	Player(sftools::Chronometer &clock);
	~Player();
	void draw(sf::RenderWindow &window) override;
	sf::FloatRect GetGlobalBounds() override;
	void SetPosition(double x, double y) override;
	
	void HandleKeyInput(Map &map);

	Dir dir = Dir::Down;
	double x = 300, y = 300;
	double speed = 1.5;
	int counter = 0;
	const int counterMax = 25;
	sftools::Chronometer &clock;
	sf::Int32 lastTime;

private:
	void move(Dir dir, double &newX, double &newY) const;

	sf::Sprite sprite;
	sf::Texture texture;
	TileSize tilesize;
};
