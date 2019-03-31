#pragma once
#include "StateHandler.hpp"

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
	StateHandler m_stateHandler;
};
