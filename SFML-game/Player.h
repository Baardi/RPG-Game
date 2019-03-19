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
	sf::FloatRect getGlobalBounds() const override;
	sf::Vector2<double> getPosition() const override;
	void setPosition(double x, double y) override;
	void takeItem(ObjectSprite *item);
	void handleKeyInput(Map &map);


private:
	sf::Vector2<double> move(Dir dir, const double prevX, const double prevY) const;

	double x = 400, y = 400;

	Dir m_dir = Dir::Down;
	double m_speed = 1.5;
	int m_counter = 0;
	const int m_counterMax = 25;
	sf::Int32 m_lastTime;
	
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	TileSize m_tilesize;
	Inventory m_inventory;
	Stats m_stats;

	std::map<Dir, sf::Keyboard::Key> m_dirMap;
	std::map<Action, sf::Keyboard::Key> m_actionMap;
};
