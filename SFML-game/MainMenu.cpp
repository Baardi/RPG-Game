#include "stdafx.h"
#include "MainMenu.hpp"
#include "State.hpp"
#include "Game.hpp"

void MainMenu::init()
{
	if (State::IsRunning())
		addMenuItem("Resume", State::Pop);

	addMenuItem("New Game", State::Reset<Game>);
	addMenuItem("Exit", State::Exit);
}
