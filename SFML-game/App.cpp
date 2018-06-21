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
	state.Setup(&window, &event, &font);
	state.Set(Transition::Push, new MainMenu);
}


void App::run()
{
	while(frame()){} // Game will exit on false, which is when the window closes
}


bool App::frame()
{
	window.clear(sf::Color::Black);

	// Todo cooldown for keypress when switching state	
	if (state.GetTransition() != Transition::None)
		SwitchState();

	if (!state.IsRunning())
		return false;

	state.GetUI()->frame();
	state.GetUI()->HandleWindowEvents();

	window.display();
	
	return true;
}

void App::SwitchState()
{
	std::cout << std::to_string(state.Size()) + std::string(" -> ");

	if (state.IsRunning())
		state.GetUI()->pause();
	
	state.CompleteTransition();
	
	if (state.IsRunning())
		state.GetUI()->resume();

	std::cout << std::to_string(state.Size()) << std::endl;
}