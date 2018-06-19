#include "stdafx.h"
#include "Player.h"


Player::Player() : tilesize(32)
{
	texture.loadFromFile("data/player_red.png");

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	sprite.setPosition(x, y);
}

Player::~Player()
{
}

void Player::draw(sf::RenderWindow &window)
{
	window.draw(sprite);
}

void Player::HandleKeyInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		move(Dir::Down);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		move(Dir::Up);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		move(Dir::Left);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		move(Dir::Right);
	else
	{
		counter = 0;
		sprite.setTextureRect(sf::IntRect(tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	}
}

void Player::move(Dir dir)
{
	this->dir = dir;
	counter = (counter + 1) % 100;

	switch (dir)
	{
	case Dir::Down:
		y += speed;
		break;

	case Dir::Up:
		y -= speed;
		break;

	case Dir::Left:
		x -= speed;
		break;

	case Dir::Right:
		x += speed;
		break;
	}

	sprite.setTextureRect(sf::IntRect((counter/25) * tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	sprite.setPosition(x, y);
}
