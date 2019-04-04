#pragma once
#include "App/Ui/StateMachine.hpp"

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
	ui::StateMachine m_stateMachine;
};
