#include "stdafx.h"
#include "App.h"
#include <iostream>
#include "Machine.h"
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
	Machine::Set(Transition::Push, State::MainMenu);
}


void App::run()
{
	while(frame()){} //game will exit on false, which is when the window closes
}


bool App::frame()
{
	// Todo cooldown for keypress when switching state	
	
	// Handle transitions
	if (Machine::GetTransition() != Transition::None)
		SwitchState();

	if (!Machine::IsRunning())
		return false;

	// Game loop
	Machine::GetUI()->frame();
	Machine::GetUI()->HandleWindowEvents();

	// Drawing (needs it's own thread, and should stop being singleton)
	window.clear(sf::Color::Black);
	Machine::GetUI()->draw();
	window.display();
	
	return true;
}

void App::SwitchState()
{
	std::cout << std::to_string(Machine::Size()) << std::string(" -> ");

	if (Machine::IsRunning())
		Machine::GetUI()->pause();

	if (Machine::GetTransition() == Transition::Pop)
		Machine::Pop();

	else if (Machine::GetTransition() == Transition::Reset)
		Machine::Reset();

	switch (Machine::GetState())
	{

	case State::Game:
		Machine::Push(new Game(window, event, font));
		break;

	case State::MainMenu:
		Machine::Push(new MainMenu(window, event, font));
		break;

	default:	// State::None causes no new UI to be pushed
		break;
	}

	if (Machine::IsRunning())
		Machine::GetUI()->resume();

	Machine::Complete();
	std::cout << std::to_string(Machine::Size()) << std::endl;
}