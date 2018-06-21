#include "stdafx.h"
#include "MainMenu.h"
#include "State.h"
#include "Game.h"

MainMenu::MainMenu()
{
}

void MainMenu::init()
{
	if (state->IsRunning())
		INDEX_RESUME = AddMenuItem("Resume");

	INDEX_NEWGAME = AddMenuItem("New Game");
	INDEX_EXIT = AddMenuItem("Exit");
}

void MainMenu::SelectEntry()
{
	if (menuIndex == INDEX_NEWGAME)
	{
		state->Set(Transition::Reset, new Game);
	}
	else if (menuIndex == INDEX_RESUME)
	{
		state->Set(Transition::Pop);
	}
	else if (menuIndex == INDEX_EXIT)
	{
		state->Set(Transition::Exit);
	}
}
