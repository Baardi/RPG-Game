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
	m_stateMachine.pushState<ui::MainMenu>();
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

	if (m_stateMachine.inTransition())
	{
		m_stateMachine.performTransition();
		std::cout << clock.getElapsedTime().asSeconds() << std::endl;
	}

	if (!m_stateMachine.isRunning())
		return false;

	auto currentUi = m_stateMachine.currentUiState();
	
	m_stateMachine.handleWindowEvents();
	if (m_stateMachine.isRespondable())
		currentUi->frame();
	
	currentUi->drawAll();

	return true;
}
