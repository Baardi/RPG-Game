#include "stdafx.h"
#include "App.h"
#include <iostream>
#include "State.h"
#include "Game.h"
#include "MainMenu.h"

App::App()
{
	window.create(sf::VideoMode(960, 960), "RPG");
	window.setFramerateLimit(100); // <-- should be a setting

	State::Setup(&window, &event, &font);
	State::Set(Transition::Push, new MainMenu);

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
}


void App::run()
{
	while(frame()){} //game will exit on false, which is when the window closes
}


bool App::frame()
{
	window.clear(sf::Color::Black);

	// Todo cooldown for keypress when switching state	
	if (State::GetTransition() != Transition::None)
		SwitchState();

	if (!State::IsRunning())
		return false;

	State::GetUI()->frame();
	State::GetUI()->HandleWindowEvents();

	window.display();
	
	return true;
}

void App::SwitchState()
{
	std::cout << std::to_string(State::Size()) + std::string(" -> ");

	if (State::IsRunning())
		State::GetUI()->pause();
	
	State::CompleteTransition();
	
	if (State::IsRunning())
		State::GetUI()->resume();

	std::cout << std::to_string(State::Size()) << std::endl;
}