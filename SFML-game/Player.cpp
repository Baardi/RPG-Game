#include "stdafx.h"
#include "Player.h"
#include <iostream>


Player::Player(sftools::Chronometer &clock) : clock(clock)
{
	texture.loadFromFile("data/player_red.png");
	tilesize.x = texture.getSize().x / 4;
	tilesize.y = texture.getSize().y / 4;

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

sf::FloatRect Player::GetGlobalBounds()
{
	return sprite.getGlobalBounds();
}

void Player::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	sprite.setPosition(x, y);
}

void Player::HandleKeyInput()
{
	bool isMoving = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		move(Dir::Down);
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		move(Dir::Up);
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		move(Dir::Left);
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		move(Dir::Right);
		isMoving = true;
	}

	if (isMoving)
	{
		counter = (counter + 1) % counterMax;
		sprite.setTextureRect(sf::IntRect(int(float(counter) / (float(counterMax) / 4.0)) * tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	}
	else
	{
		counter = counter/counterMax;
		sprite.setTextureRect(sf::IntRect(tilesize.x, int(dir) * tilesize.y, tilesize.x, tilesize.y));
	}
}

void Player::move(Dir dir)
{
	this->dir = dir;

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

	sprite.setPosition(x, y);
}
