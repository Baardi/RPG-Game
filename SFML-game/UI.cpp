#include "stdafx.h"
#include "UI.h"
#include "Machine.h"

bool UI::PollEvent(sf::Event::EventType eventType)
{
	switch (eventType)
	{
	case sf::Event::Closed:
		Machine::Set(Transition::Exit);
		return true;

		// causes an unresumable pause in menus
		/*		case sf::Event::EventType::LostFocus:
		if (Machine::IsRunning())
		Machine::GetUI()->pause();*/
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
