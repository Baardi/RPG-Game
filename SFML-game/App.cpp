#include "stdafx.h"
#include "App.h"
#include <iostream>
#include "State.h"
#include "Game.h"
#include "MainMenu.h"

App::App()
{
	window.create(sf::VideoMode(960, 960), "RPG");
	window.setFramerateLimit(50); // <-- should be a setting
	if (!font.loadFromFile("data/Asul-regular.ttf"))
	{
		std::cout << "file can not be loaded, sorry!" << std::endl;
	}
}


App::~App()
{
}

void App::init()
{
	State::Setup(window, event, font);
	State::Set(Transition::Push, new MainMenu);
}


void App::run()
{
	init();
	while(frame()){} // Game will exit on false, which is when the window closes
}


bool App::frame()
{
	// Todo cooldown for keypress when switching state	
	if (State::IsInTransition())
		SwitchState();

	if (!State::IsRunning())
		return false;

	State::GetUI()->frame();
	State::GetUI()->HandleWindowEvents();

	// Drawing (needs it's own thread, and should stop being singleton)
	window.clear(sf::Color::Black);
	State::GetUI()->drawAll();
	window.display();
	
	return true;
}

void App::SwitchState()
{
	std::cout << std::to_string(State::Size()) + std::string(" -> ");

	if (State::IsRunning())
		State::GetUI()->pause();
	
	State::PerformTransition();
	
	if (State::IsRunning())
		State::GetUI()->resume();

	std::cout << std::to_string(State::Size()) << std::endl;
}
