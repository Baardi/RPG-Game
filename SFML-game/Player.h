#pragma once
#include "Character.hpp"
#include "Inventory.h"
#include "ObjectSprite.h"

class Map;

class Player : public Character
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
	void draw(sf::RenderTarget &window) override;
	sf::DoubleRect GetGlobalBounds() const override;
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

	sf::Sprite m_sprite;
	sf::Texture m_texture;
	TileSize m_tilesize;
	Inventory m_inventory;
	Stats m_stats;

	std::map<Dir, sf::Keyboard::Key> m_dirMap;
	std::map<Action, sf::Keyboard::Key> m_actionMap;
};
