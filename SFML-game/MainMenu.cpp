#include "stdafx.h"
#include "MainMenu.h"
#include "State.h"
#include "Game.h"

void MainMenu::init()
{
	if (State::IsRunning())
		AddMenuItem("Resume", State::Pop);

	AddMenuItem("New Game", State::Reset<Game>);
	AddMenuItem("Exit", State::Exit);
}
