#include "stdafx.h"
#include "Player.h"
#include "map.h"

Player::Player(sftools::Chronometer &clock, int x, int y) : clock(clock)
{
	texture.loadFromFile("data/player_red.png");
	tilesize.x = texture.getSize().x / 4;
	tilesize.y = texture.getSize().y / 4;

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	
	// Map keys to directions for the player
	dirMap.emplace(Dir::Left, sf::Keyboard::Key::Left);
	dirMap.emplace(Dir::Right, sf::Keyboard::Key::Right);
	dirMap.emplace(Dir::Up, sf::Keyboard::Key::Up);
	dirMap.emplace(Dir::Down, sf::Keyboard::Key::Down);
	
	// Map keys for actions (such as open inventory)
	actionMap.emplace(Action::Inventory, sf::Keyboard::Key::I);

	Player::SetPosition(x, y);
}

Player::~Player()
{
}

void Player::draw(sf::RenderWindow &window)
{
	window.draw(sprite);
}

sf::FloatRect Player::GetGlobalBounds()
{
	return sprite.getGlobalBounds();
}

void Player::SetPosition(double x, double y)
{
	this->x = x;
	this->y = y;
	sprite.setPosition(x, y);
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
	
	if (sf::Keyboard::isKeyPressed(actionMap[Action::Inventory])) // Inventory &inventory = dynamic_cast<Game *>(GetParent())->GetPlayer()->GetInventory() should be used inside Ui header
		/*State::PushChild<InventoryUI>()*/; // Inventory popup
	if (sf::Keyboard::isKeyPressed(actionMap[Action::Talk]))
		;// Get sprite-id, then start the dialog tree that matches that sprite id. Give necessary parameters

	if (isMoving)
	{
		auto unWalkables = map.GetTileLayer("Unwalkables");
		auto Walkables = map.GetTileLayer("Walkables");

		double newX, newY;
		move(dir, x, y, newX, newY);

		if (! (unWalkables && unWalkables->containsTexture(newX, newY))
			|| (Walkables &&  Walkables->containsTexture(newX, newY)) )
			SetPosition(newX, newY);
		
		counter = (counter + 1) % counterMax;
		sprite.setTextureRect(sf::IntRect(int(float(counter) / (float(counterMax) / 4.0)) * tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	}
	else
	{
		counter = counter/counterMax;
		sprite.setTextureRect(sf::IntRect(tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
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
