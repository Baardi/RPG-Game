#include "stdafx.h"
#include "MainMenu.h"
#include "State.h"
#include "Game.h"

void MainMenu::init()
{
	if (State::IsRunning())
		addMenuItem("Resume", State::Pop);

	addMenuItem("New Game", State::Reset<Game>);
	addMenuItem("Exit", State::Exit);
}
