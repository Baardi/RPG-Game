#include "stdafx.h"
#include "App.hpp"
#include <iostream>
#include "MainMenu.hpp"
#include "ResourceHandler.hpp"

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

	if (!resourceHandler().font().loadFromFile("data/Asul-regular.ttf"))
	{
		std::cout << "file can not be loaded, sorry!" << std::endl;
	}

	m_stateHandler.setWindow(m_window);
	m_stateHandler.pushState<MainMenu>();
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

	if (m_stateHandler.inTransition())
	{
		m_stateHandler.performTransition();
		std::cout << clock.getElapsedTime().asSeconds() << std::endl;
	}

	if (!m_stateHandler.isRunning())
		return false;

	auto currentUi = m_stateHandler.getCurrentUI();
	
	m_stateHandler.handleWindowEvents();
	if (m_stateHandler.isRespondable())
		currentUi->frame(m_window);
	
	currentUi->drawAll();

	return true;
}
