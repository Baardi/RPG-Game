#include "stdafx.h"
#include "App.hpp"
#include <iostream>
#include "MainMenu.hpp"
#include "ResourceManager.hpp"

App::App()
{
}

App::~App()
{
}

void App::init()
{
	m_window.create(sf::VideoMode(960, 960), "RPG");
	m_window.setFramerateLimit(50); // <-- should be a setting

	if (!resources().font().loadFromFile("data/Asul-regular.ttf"))
	{
		std::cout << "file can not be loaded, sorry!" << std::endl;
	}

	m_stateMachine.setWindow(m_window);
	m_stateMachine.pushState<appstate::MainMenu>();
}

void App::run()
{
	init();
	while (frame()); // Game will exit on false, which is when the window closes
}

bool App::frame()
{
	return m_stateMachine.runFrame();
}
