#include "stdafx.h"
#include "Player.hpp"
#include "map.hpp"
#include "State.hpp"
#include "InventoryUI.hpp"
#include "DialogInterface.hpp"

Player::Player(sftools::Chronometer &clock, int x, int y)
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
	this->x = x;
	this->y = y;
	m_sprite.setPosition(x, y);
}

void Player::setPosition(sf::Vector2<double> pos)
{
	this->x = pos.x;
	this->y = pos.y;
	m_sprite.setPosition(static_cast<sf::Vector2f>(pos));
}

sf::Vector2<double> Player::getPosition() const
{
	return sf::Vector2(x, y);
}

void Player::takeItem(std::unique_ptr<GameItem> &&item)
{
	m_inventory.takeItem(std::move(item));
}

void Player::handleKeyInput(Map &map)
{
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
	{
		State::PushChild<InventoryUI>(); // Inventory popup
		State::SetInitializer<InventoryInitializer>(m_inventory);
	}
	if (sf::Keyboard::isKeyPressed(m_actionMap[Action::Talk]))
		State::PushChild<DialogInterface>(); // Inventory popup;// Get sprite-id, then start the dialog tree that matches that sprite id. Give necessary parameters

	if (isMoving)
	{
		auto [newX, newY] = move(m_dir, x, y);

		auto unWalkables = map.getTileLayer("Unwalkables");
		auto walkables = map.getTileLayer("Walkables");

		if (!(unWalkables && unWalkables->containsTexture(newX, newY)) || (walkables && walkables->containsTexture(newX, newY)))
			setPosition(newX, newY);
		
		m_counter = (m_counter + 1) % m_counterMax;
		m_sprite.setTextureRect(sf::IntRect(int(float(m_counter) / (float(m_counterMax) / 4.0)) * m_tilesize.x, int(m_dir) * m_tilesize.y, m_tilesize.x, m_tilesize.y));
	}
	else
	{
		m_counter = m_counter/m_counterMax;
		m_sprite.setTextureRect(sf::IntRect(0, int(m_dir) * m_tilesize.y, m_tilesize.x, m_tilesize.y));
	}
}

sf::Vector2<double> Player::move(Dir dir, const double prevX, const double prevY) const
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

	return sf::Vector2(newX, newY);
}
