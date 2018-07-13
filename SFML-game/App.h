#pragma once
#include "State.h"

class App
{
public:
	App();
	~App();
	void init(); //set startup state etc
	void run();  //run the game loop

private:
	bool frame(); //Draws one gameframe. Returns false if window is closed, which will end the gameloop

	sf::Font font;
	sf::Event event;
	sf::RenderWindow window;
};
