#pragma once
#include "TileLayer.h"
#include "GameObject.h"
#include "Inventory.h"
#include "ObjectSprite.h"


class Player : public GameObject
{
public:
enum class Dir
{
	Down = 0,
	Left = 1,
	Right = 2,
	Up = 3,
};

enum class Action
{
	Talk, // Right?
	Inventory
};
	Player(sftools::Chronometer &clock, int x, int y);
	~Player();
	void draw(sf::RenderWindow &window) override;
	sf::DoubleRect GetGlobalBounds() override;
	void SetPosition(double x, double y) override;
	void TakeItem(ObjectSprite *item);
	void HandleKeyInput(Map &map);

	Dir dir = Dir::Down;
	double x = 400, y = 400;
	double speed = 1.5;
	int counter = 0;
	const int counterMax = 25;
	sftools::Chronometer &clock;
	sf::Int32 lastTime;

private:
	void move(Dir dir, const double prevX, const double prevY, double &newX, double &newY) const;

	sf::Sprite sprite;
	sf::Texture texture;
	TileSize tilesize;
	Inventory inventory;

	std::map<Dir, sf::Keyboard::Key> dirMap;
	std::map<Action, sf::Keyboard::Key> actionMap;
};
