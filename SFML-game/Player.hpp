#pragma once
#include "Entity.hpp"
#include "Inventory.hpp"
#include "ObjectSprite.hpp"

class Map;
namespace appstate { class Game; }

class Player : public GameObject, public Entity
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
	
	sf::FloatRect getLocalBounds() const override;
	sf::Transform getTransform() const override;

	sf::Vector2<double> getPosition() const override;
	void setPosition(double x, double y) override;
	void setPosition(sf::Vector2<double> pos) override;

	void draw(sf::RenderTarget &target) override;

	void takeItem(std::unique_ptr<GameItem> &&item);
	void handleKeyInput(appstate::Game &game, Map &map);


private:
	sf::Vector2<double> move(Dir dir, const double prevX, const double prevY) const;

	double x = 400, y = 400;

	Dir m_dir = Dir::Down;
	double m_speed = 5.5;
	int m_counter = 0;
	const int m_counterMax = 25;
	sf::Int32 m_lastTime;
	
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	TileSize m_tilesize;
	Inventory m_inventory;

	std::map<Dir, sf::Keyboard::Key> m_dirMap;
	std::map<Action, sf::Keyboard::Key> m_actionMap;
};
