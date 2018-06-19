#pragma once
#include <SFML/Graphics.hpp>
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
	Player();
	~Player();
	void draw(sf::RenderWindow &window) override;
	void HandleKeyInput();

	Dir dir = Dir::Down;
	float x = 300, y = 300;
	float speed = 0.75;
	int counter = 0;

private:
	void move(Dir dir);
	sf::Sprite sprite;
	sf::Texture texture;
	TileSize tilesize;
};
