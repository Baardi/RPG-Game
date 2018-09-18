#include "stdafx.h"
#include "MainMenu.h"
#include "State.h"
#include "MainGame.h"

void MainMenu::init()
{
	if (State::IsRunning())
		AddMenuItem("Resume", State::Pop);

	AddMenuItem("New Game", State::Reset<MainGame>);
	AddMenuItem("Exit", State::Exit);
}
