#include "stdafx.h"
#include "App.hpp"
#include <iostream>
#include "State.hpp"
#include "MainMenu.hpp"


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
	if (!State::Instance().font.loadFromFile("data/Asul-regular.ttf"))
	{
		std::cout << "file can not be loaded, sorry!" << std::endl;
	}

	State::Push<MainMenu>();
}

void App::run()
{
	init();
	while (frame()); // Game will exit on false, which is when the window closes
}

bool App::frame()
{
	// Todo cooldown for keypress when switching state
	
	sftools::Chronometer clock;
	clock.reset(true);
	
	if (State::IsInTransition())
	{
		State::PerformTransition();
		std::cout << clock.getElapsedTime().asSeconds() << std::endl;
	}

	if (!State::IsRunning())
		return false;

	State::GetUI()->handleWindowsEvents(m_window);
	
	State::GetUI()->drawAll(m_window);
	
	if (State::GetUI()->isRespondable())
		State::GetUI()->frame(m_window);

	return true;
}
