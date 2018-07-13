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
	// Reverse iteration, so the parent is drawn in backround
	for (auto it = drawStack.rbegin(); it != drawStack.rend(); ++it)
		(*it)->draw();
}

bool UI::PollEvent(sf::Event::EventType eventType)
{
	switch (eventType)
	{
	case sf::Event::Closed:
		State::Set(Transition::Exit);
		return true;

		// causes an unresumable pause in menus
		/*		case sf::Event::EventType::LostFocus:
		if (State::IsRunning())
		State::GetUI()->pause();*/
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
