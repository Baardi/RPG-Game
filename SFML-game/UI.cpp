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
		if (State::IsRunning())
			State::GetUI()->pause();

		return true;

	default:
		return false;
	}
}

void UI::HandleWindowEvents()
{
	while (window.pollEvent(event))
	{
		PollEvent(event.type);
	}
}
