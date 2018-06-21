#include "stdafx.h"
#include "UI.h"
#include "State.h"

void UI::init()
{
}

bool UI::PollEvent(sf::Event::EventType eventType)
{
	switch (eventType)
	{
	case sf::Event::Closed:
		state->Set(Transition::Exit);
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
	while (window->pollEvent(*event))
	{
		PollEvent(event->type);
	}
}

void UI::Setup(State *state, sf::RenderWindow *window, sf::Event *event, sf::Font *font)
{
	this->window = window;
	this->event = event;
	this->font = font;
	this->state = state;
}
