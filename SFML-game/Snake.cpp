#include "stdafx.h"
#include "Snake.h"
#include <iostream>


Snake::Snake(sf::RenderWindow &window, sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight, sf::Color color, double xPos, double yPos) : window(window), head(window, color)
{
	this->keyLeft = keyLeft;
	this->keyRight = keyRight;
	this->color = color;

	random = std::rand() % 10;

	head.xPos = xPos;
	head.yPos = yPos;
}

Snake::~Snake()
{
	for (auto elem : body)
		delete elem;
}

void Snake::move()
{
	if (body.size())
	{
		//Update from back to front
		for (int i = body.size() - 1; i > 0; i--)
			body[i]->setPosition(body[i - 1]->xPos, body[i - 1]->yPos);

		//first body elem, follows head
		body[0]->setPosition(head.xPos, head.yPos);
	}

	head.move(xVector, yVector, speed);
	xPos = head.xPos;
	yPos = head.yPos;
}


void Snake::grow()
{
	for (int i = 0; i < 10;i++)
	{
		if (body.size())
			body.push_back(new SnakeBodyElem(window, !random ? sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255) : color, body.back()->xPos, body.back()->yPos));
		else
			body.push_back(new SnakeBodyElem(window, !random ? sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255) : color, head.xPos, head.yPos));
	}
	speed += 0.2;
}

void Snake::respawn()
{
	isAlive = true;
	speed = 5;
	
	for (auto elem : body)
		delete elem;

	body.clear();

	head.setPosition(490, 490);
}

void Snake::despawn()
{
	isAlive = false;
	speed = 5;

	for (auto elem : body)
		delete elem;

	body.clear();

	head.setPosition(-9999, -9999);
}

void Snake::draw()
{
	if (!isAlive)
		return;

	head.draw();

	for (auto elem : body)
		elem->draw();
}

bool Snake::SelfCollides()
{
	for (int i = 30; i < body.size(); i++)//30 seems to be a reasonable number, for wanted behavior
	{
		auto elem = body[i];
		if (head.Intersects(*elem))
			return true;
	}

	//std::cout << "Game over" << std::endl;
	return false;
}

bool Snake::Collides(GameObject &object)
{
	return head.Intersects(object);
}

bool Snake::Collides(Snake &snake)
{
	if (&snake == this)
		return SelfCollides();

	if (Collides(snake.head))
		return true;

	for (auto elem : snake.body)
	{
		if (Collides(*elem))
			return true;
	}

	return false;
}

void Snake::updateAngle(int direction)
{
	angle += direction * tau / 48;

	if (angle > 100 * tau)
		angle -= 100 * tau;
	else if (angle < -100 * tau)
		angle += 100 * tau;

	xVector = cos(angle);
	yVector = sin(angle);
}

void Snake::HandleKeyInput()
{
	if (sf::Keyboard::isKeyPressed(keyLeft))
		updateAngle(-1);

	if (sf::Keyboard::isKeyPressed(keyRight))
		updateAngle(1);
}

int Snake::GetScore()
{
	return body.size() / 10;
}


Score::Score(sf::RenderWindow &window, sf::Color color, sf::Font &font, double xPos, double yPos) : text("0", font, 30), window(window)
{
	text.setPosition(xPos, yPos);
	text.setFillColor(color);
}

void Score::SetScore(int score)
{
	text.setString(std::to_string(score));
}

void Score::draw()
{
	window.draw(text);
}

Player::Player(sf::RenderWindow &window, sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight, sf::Color color, sf::Font &font, int playerNumber)
: score(window, color, font, 0, 40 * playerNumber)
{
	snake = new Snake(window, keyLeft, keyRight, color, 240 + 500 * (playerNumber % 2), 240 + 500 * (playerNumber / 2));
}

Player::~Player()
{
	delete snake;
}

void Player::draw()
{
	snake->draw();
	score.draw();	
}

void Player::SetScore()
{
	score.SetScore(snake->GetScore());
}

