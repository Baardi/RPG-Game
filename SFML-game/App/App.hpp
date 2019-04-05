#pragma once
#include "App/StateMachine.hpp"

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
	appstate::StateMachine m_stateMachine;
};
