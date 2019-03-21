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
	window.create(sf::VideoMode(960, 960), "RPG");
	window.setFramerateLimit(50); // <-- should be a setting
	if (!font.loadFromFile("data/Asul-regular.ttf"))
	{
		std::cout << "file can not be loaded, sorry!" << std::endl;
	}

	State::Setup(window, event, font);
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

	State::GetUI()->HandleWindowEvents();
	State::GetUI()->drawAll();
	
	if (State::GetUI()->isRespondable())
		State::GetUI()->frame();

	return true;
}
