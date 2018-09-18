#include "stdafx.h"
#include "Player.h"
#include "map.h"
#include "State.h"
#include "InventoryUI.h"
#include "DialogInterface.h"

Player::Player(sftools::Chronometer &clock, int x, int y) : clock(clock)
{
	texture.loadFromFile("data/Player Sprites/Mage.png");
	tilesize.x = texture.getSize().x / 4;
	tilesize.y = texture.getSize().y / 4;

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	
	// Map keys to directions for the player
	dirMap.emplace(Dir::Left, sf::Keyboard::Key::Left);
	dirMap.emplace(Dir::Right, sf::Keyboard::Key::Right);
	dirMap.emplace(Dir::Up, sf::Keyboard::Key::Up);
	dirMap.emplace(Dir::Down, sf::Keyboard::Key::Down);
	
	// Map keys for actions (such as open inventory)
	actionMap.emplace(Action::Inventory, sf::Keyboard::Key::I);
	actionMap.emplace(Action::Talk, sf::Keyboard::Key::T);

	Player::SetPosition(x, y);
}

Player::~Player()
{
}

void Player::draw(sf::RenderWindow &window)
{
	window.draw(sprite);
}

sf::DoubleRect Player::GetGlobalBounds()
{
	return static_cast<sf::DoubleRect>(sprite.getGlobalBounds());
}

void Player::SetPosition(double x, double y)
{
	this->x = x;
	this->y = y;
	sprite.setPosition(x, y);
}

void Player::TakeItem(int gid, const std::string &itemName)
{
	inventory.AddItem(gid, itemName);
}

void Player::HandleKeyInput(Map &map)
{
	bool isMoving = false;

	for (auto kvPair : dirMap)
	{
		if (sf::Keyboard::isKeyPressed(kvPair.second))
		{
			dir = kvPair.first;
			isMoving = true;
			break;
		}
	}
	
	if (sf::Keyboard::isKeyPressed(actionMap[Action::Inventory]))
	{
		State::PushChild<InventoryUI>(); // Inventory popup
		InventoryInitializer *initializer = new InventoryInitializer;
		initializer->inventory = &inventory;
		State::SetInitializer(initializer);
	}
	if (sf::Keyboard::isKeyPressed(actionMap[Action::Talk]))
		State::PushChild<DialogInterface>(); // Inventory popup;// Get sprite-id, then start the dialog tree that matches that sprite id. Give necessary parameters

	if (isMoving)
	{
		double newX, newY;
		move(dir, x, y, newX, newY);

		auto unWalkables = map.GetTileLayer("Unwalkables");
		if (!(unWalkables && unWalkables->containsTexture(newX, newY)))
			SetPosition(newX, newY);
		
		counter = (counter + 1) % counterMax;
		sprite.setTextureRect(sf::IntRect(int(float(counter) / (float(counterMax) / 4.0)) * tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	}
	else
	{
		counter = counter/counterMax;
		sprite.setTextureRect(sf::IntRect(0, int(dir) * tilesize.y, tilesize.x, tilesize.y));
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
