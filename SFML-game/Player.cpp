#include "stdafx.h"
#include "Player.h"
#include "map.h"

Player::Player(sftools::Chronometer &clock) : clock(clock)
{
	texture.loadFromFile("data/player_red.png");
	tilesize.x = texture.getSize().x / 4;
	tilesize.y = texture.getSize().y / 4;

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		dir = Dir::Down;
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		dir = Dir::Up;
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		dir = Dir::Left;
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		dir = Dir::Right;
		isMoving = true;
	}
	

	if (isMoving)
	{
		double newX, newY;
		move(dir, newX, newY);

		if (map.isWalkableScreenCoords(newX, newY))
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

void Player::move(Dir dir, double &newX, double &newY) const
{
	newX = x;
	newY = y;

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
