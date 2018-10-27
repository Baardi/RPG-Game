#include "stdafx.h"
#include "UI.h"
#include "State.h"

UI::UI() : window(*State::Instance().window),
			event(*State::Instance().event),
			font(*State::Instance().font)
{
}

void UI::init()
{
}

void UI::setDrawOrder()
{
	drawStack.clear();
	for (auto curr = this; curr; curr = curr->parent)
		drawStack.push_back(curr);
}

void UI::drawAll()
{
	window.clear(sf::Color::Black);

	// Reverse iteration, so the parent is drawn in backround
	for (auto it = drawStack.rbegin(); it != drawStack.rend(); ++it)
		(*it)->draw();
	
	window.display();
}

bool UI::PollEvent(sf::Event::EventType eventType)
{
	switch (eventType)
	{
	case sf::Event::Closed:
		State::Exit();
		return true;

	case sf::Event::LostFocus:
		pause();
		respondable = false;
		return true;
		
	case sf::Event::GainedFocus:
		respondable = true;
		return true;

	default:
		return false;
	}
}

void UI::HandleWindowEvents()
{
	while (window.pollEvent(event))
	{
		if (respondable)
			// Normal virtual pollevent, overriding ui's can hook into
			PollEvent(event.type);
		else
			// Don't call the virtual method, just check if the window is in a respondable state
			UI::PollEvent(event.type); 
	}
}

void UI::pause()
{
	if (pausable)
		paused = true;
}

void UI::resume()
{
	if (pausable)
		paused = false;
}

void UI::toggle()
{
	if (pausable)
		paused ? resume() : pause();
}
