#include "stdafx.h"
#include "Player.h"
#include "map.h"
#include "State.h"
#include "InventoryUI.h"
#include "DialogInterface.h"

Player::Player(sftools::Chronometer &clock, int x, int y) : clock(clock)
{
	m_texture.loadFromFile("data/Player Sprites/Warrior.png");
	m_tilesize.x = m_texture.getSize().x / 4;
	m_tilesize.y = m_texture.getSize().y / 4;

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, int(dir) * m_tilesize.y, m_tilesize.x, m_tilesize.y));
	
	// Map keys to directions for the player
	m_dirMap.emplace(Dir::Left,	 sf::Keyboard::Key::Left);
	m_dirMap.emplace(Dir::Right,	 sf::Keyboard::Key::Right);
	m_dirMap.emplace(Dir::Up,		 sf::Keyboard::Key::Up);
	m_dirMap.emplace(Dir::Down,	 sf::Keyboard::Key::Down);
	
	// Map keys for actions (such as open inventory)
	m_actionMap.emplace(Action::Inventory,	sf::Keyboard::Key::I);
	m_actionMap.emplace(Action::Talk,			sf::Keyboard::Key::T);
	
	Player::SetPosition(x, y);
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget &window)
{
	window.draw(m_sprite);
}

sf::FloatRect Player::GetGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

void Player::SetPosition(double x, double y)
{
	this->x = x;
	this->y = y;
	m_sprite.setPosition(x, y);
}

void Player::takeItem(ObjectSprite *item)
{
	m_inventory.AddItem(item);
}

void Player::HandleKeyInput(Map &map)
{
	bool isMoving = false;

	for (auto kvPair : m_dirMap)
	{
		if (sf::Keyboard::isKeyPressed(kvPair.second))
		{
			dir = kvPair.first;
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
		double newX, newY;
		move(dir, x, y, newX, newY);

		auto unWalkables = map.GetTileLayer("Unwalkables");
		auto Walkables = map.GetTileLayer("Walkables");
		if (!(unWalkables && unWalkables->containsTexture(newX, newY)) ||
			 (  Walkables &&   Walkables->containsTexture(newX, newY)))
			SetPosition(newX, newY);
		
		counter = (counter + 1) % counterMax;
		m_sprite.setTextureRect(sf::IntRect(int(float(counter) / (float(counterMax) / 4.0)) * m_tilesize.x, int(dir) * m_tilesize.y, m_tilesize.x, m_tilesize.y));
	}
	else
	{
		counter = counter/counterMax;
		m_sprite.setTextureRect(sf::IntRect(0, int(dir) * m_tilesize.y, m_tilesize.x, m_tilesize.y));
	}
}

void Player::move(Dir dir, const double prevX, const double prevY, double &newX, double &newY) const
{
	newX = prevX;
	newY = prevY;

	switch (dir)
	{
	case Dir::Down:
		newY += speed;
		break;

	case Dir::Up:
		newY -= speed;
		break;

	case Dir::Left:
		newX -= speed;
		break;

	case Dir::Right:
		newX += speed;
		break;
	}
}
