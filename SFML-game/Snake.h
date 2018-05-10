#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "SnakeHead.h"
#include "SnakeBodyElem.h"

class Snake
{
public:
	Snake(sf::RenderWindow &window, sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight, sf::Color color, double xPos, double yPos);
	~Snake();
	void move();
	void grow();
	void respawn();
	void despawn();
	void draw();
	bool SelfCollides();
	bool Collides(GameObject &object);
	bool Collides(Snake &snake);
	void updateAngle(int direction); //1: clockwise, -1; anticlockwise
	void HandleKeyInput();
	int GetScore();

	bool isAlive = true;
	double angle = 0;
	double xVector = cos(angle);
	double yVector = sin(angle);
	double xPos, yPos;
private:
	sf::RenderWindow &window;
	sf::Keyboard::Key keyLeft;
	sf::Keyboard::Key keyRight;
	sf::Color color;

	SnakeHead head;
	std::vector<SnakeBodyElem*> body;

	const double tau = 8 * atan(1); // <-- shouldnt be here
	double speed = 5;
	bool random;
};

class Score
{
public:
	Score(sf::RenderWindow &window, sf::Color color, sf::Font &font, double xPos, double yPos);

	void SetScore(int score);
	void draw();
	sf::Text text;
private:
	sf::RenderWindow &window;
};

class Player
{
public:
	Player(sf::RenderWindow &window, sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight, sf::Color color, sf::Font &font, int playerNumber);
	~Player();

	Score score;
	Snake *snake;

	void draw();
	void SetScore();
};