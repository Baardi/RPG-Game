#pragma once
#include "State.hpp"

class App
{
public:
	App();
	~App();

	void run();
	void init();
	bool frame();

private:
	sf::RenderWindow m_window;

	// Todo: move font and texturemap (resources) from state to app
};
