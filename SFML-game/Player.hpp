#pragma once
#include "Entity.hpp"
#include "Inventory.hpp"
#include "ObjectSprite.hpp"
#include "MouseHandler.hpp"
#include "KeyHandler.hpp"

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
	Player(sftools::Chronometer &clock, sf::Vector2f pos);
	~Player();
	
	sf::FloatRect getLocalBounds() const override;
	sf::Transform getTransform() const override;

	sf::Vector2f getPosition() const override;
	void setPosition(sf::Vector2f pos) override;

	void draw(sf::RenderTarget &target) override;
	void drawToWindow(sf::RenderTarget &target);

	void fight(Entity& other) override;

	void takeItem(std::unique_ptr<GameItem> item);
	void handleKeyInput(appstate::Game &game, Map &map);


private:
	sf::Vector2f move(Dir dir, sf::Vector2f prevPos) const;

	Dir m_dir = Dir::Down;
	float m_speed = 5.5;
	int m_counter = 0;
	const int m_counterMax = 25;
	int m_lastTime;
	std::optional<sftools::Chronometer> m_fightTimer;
	
	sf::Text m_textStats;
	sf::Text m_textFighting;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	TileSize m_tilesize;
	Inventory m_inventory;

	std::map<Dir, sf::Keyboard::Key> m_dirMap;
	std::map<Action, sf::Keyboard::Key> m_actionMap;
	MouseHandler m_mouseHandler;
	KeyHandler m_keyHandler;
};
