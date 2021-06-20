#include "stdafx.h"
#include "Player.hpp"
#include "Map.hpp"
#include "InventoryUI.hpp"
#include "DialogInterface.hpp"
#include "TileLayer.hpp"
#include "Game.hpp"
#include "App/StateMachine.hpp"
#include "App/ResourceManager.hpp"

Player::Player([[maybe_unused]]sftools::Chronometer &clock, double x, double y)
{
	m_texture.loadFromFile("data/Player Sprites/Warrior.png");
	m_tilesize.x = m_texture.getSize().x / 4;
	m_tilesize.y = m_texture.getSize().y / 4;

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, int(m_dir) * m_tilesize.y, m_tilesize.x, m_tilesize.y));
	
	// Map keys to directions for the player
	m_dirMap.emplace(Dir::Left,	 sf::Keyboard::Key::Left);
	m_dirMap.emplace(Dir::Right, sf::Keyboard::Key::Right);
	m_dirMap.emplace(Dir::Up,	 sf::Keyboard::Key::Up);
	m_dirMap.emplace(Dir::Down,	 sf::Keyboard::Key::Down);
	
	// Map keys for actions (such as open inventory)
	m_actionMap.emplace(Action::Inventory,	sf::Keyboard::Key::I);
	m_actionMap.emplace(Action::Talk,		sf::Keyboard::Key::T);
	
	m_mouseHandler.onButtonPressed(sf::Mouse::Button::Left, [&]() 
		{
			if (!m_fightTimer)
				m_fightTimer.emplace().reset(true);
		});

	m_textFighting.setFont(resources().font());
	m_textFighting.setPosition(0, 0);
	m_textFighting.setCharacterSize(10);
	m_textFighting.setString("Fighting");

	m_textStats.setFont(resources().font());
	m_textStats.setPosition(0, 25);
	m_textFighting.setCharacterSize(10);

	// Put these as default stats temporarily
	m_stats.HP = 30;
	m_stats.Damage = 5;
		
	Player::setPosition(x, y);
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget &target)
{
	target.draw(m_sprite);
#ifdef _DEBUG
	drawDebugOutline(target);
#endif // _DEBUG
}

void Player::drawToWindow(sf::RenderTarget &target)
{
	target.draw(m_textStats);
	
	if (m_fightTimer)
		target.draw(m_textFighting);
}

void Player::fight(Entity& other)
{
	if (m_fightTimer)
	{
		hit(other, m_stats.Damage);
		m_fightTimer = std::nullopt;
	}
}

sf::FloatRect Player::getLocalBounds() const
{
	return m_sprite.getLocalBounds();
}

sf::Transform Player::getTransform() const
{
	return m_sprite.getTransform();
}

void Player::setPosition(double x, double y)
{
	m_x = x;
	m_y = y;
	m_sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
}

void Player::setPosition(sf::Vector2f pos)
{
	m_x = pos.x;
	m_y = pos.y;
	m_sprite.setPosition(pos);
}

sf::Vector2f Player::getPosition() const
{
	return sf::Vector2f(static_cast<float>(m_x), static_cast<float>(m_y));
}

void Player::takeItem(std::unique_ptr<GameItem> item)
{
	m_inventory.takeItem(std::move(item));
}

void Player::handleKeyInput(appstate::Game &game, Map &map)
{
	if (m_fightTimer && m_fightTimer->getElapsedTime_ms() > 300)
		m_fightTimer = std::nullopt;

	m_textStats.setString(m_stats.toString());
	m_mouseHandler.handleMouseInput();
	m_keyHandler.handleKeyInput();

	bool isMoving = false;

	for (auto [dir, key] : m_dirMap)
	{
		if (sf::Keyboard::isKeyPressed(key))
		{
			m_dir = dir;
			isMoving = true;
			break;
		}
	}
	
	if (sf::Keyboard::isKeyPressed(m_actionMap[Action::Inventory]))
		game.stateMachine().pushChild<appstate::InventoryPopup>(m_inventory); // Inventory popup
	if (sf::Keyboard::isKeyPressed(m_actionMap[Action::Talk]))
		game.stateMachine().pushChild<appstate::DialogInterface>(); // Get sprite-id, then start the dialog tree that matches that sprite id. Give necessary parameters

	if (isMoving)
	{
		auto [newX, newY] = move(m_dir, m_x, m_y);

		auto collisionLayer = map.getTileLayer("Collision");
		if ((!collisionLayer || !collisionLayer->containsTexture(newX, newY)))
		{
			setPosition(newX, newY);
			game.updateDrawRect();
		}

		m_counter = (m_counter + 1) % m_counterMax;
		m_sprite.setTextureRect(sf::IntRect(int(float(m_counter) / (float(m_counterMax) / 4.0)) * m_tilesize.x, int(m_dir) * m_tilesize.y, m_tilesize.x, m_tilesize.y));
	}
	else
	{
		m_counter = m_counter/m_counterMax;
		m_sprite.setTextureRect(sf::IntRect(0, int(m_dir) * m_tilesize.y, m_tilesize.x, m_tilesize.y));
	}
}

sf::Vector2f Player::move(Dir dir, const double prevX, const double prevY) const
{
	double newX = prevX;
	double newY = prevY;

	switch (dir)
	{
	case Dir::Down:
		newY += m_speed;
		break;

	case Dir::Up:
		newY -= m_speed;
		break;

	case Dir::Left:
		newX -= m_speed;
		break;

	case Dir::Right:
		newX += m_speed;
		break;
	}

	return sf::Vector2f(static_cast<float>(newX), static_cast<float>(newY));
}
