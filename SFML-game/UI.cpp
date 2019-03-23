#include "stdafx.h"
#include "UI.hpp"
#include "State.hpp"


void UI::init()
{
}

void UI::setDrawOrder()
{
	m_drawStack.clear();
	for (auto curr = this; curr; curr = curr->m_pParent)
		m_drawStack.push_back(curr);
}

void UI::drawAll(sf::RenderWindow &window)
{
	window.clear(sf::Color::Black);

	// Reverse iteration, so the parent is drawn in backround
	for (auto it = m_drawStack.rbegin(); it != m_drawStack.rend(); ++it)
		(*it)->draw(window);
	
	window.display();
}

bool UI::pollEvent(sf::Event::EventType eventType)
{
	switch (eventType)
	{
	case sf::Event::Closed:
		State::Exit();
		return true;

	case sf::Event::LostFocus:
		pause();
		m_respondable = false;
		return true;
		
	case sf::Event::GainedFocus:
		m_respondable = true;
		return true;

	default:
		return false;
	}
}

void UI::handleWindowsEvents(sf::Window &window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (m_respondable)
			// Normal virtual pollEvent, overriding ui's can hook into
			pollEvent(event.type);
		else
			// Don't call the virtual method, just check if the window is in a respondable state
			UI::pollEvent(event.type); 
	}
}

void UI::pause()
{
	if (m_pausable)
		m_paused = true;
}

void UI::resume()
{
	if (m_pausable)
		m_paused = false;
}

void UI::toggle()
{
	if (m_pausable)
		m_paused ? resume() : pause();
}
