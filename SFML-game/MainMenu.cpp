#include "stdafx.h"
#include "MainMenu.h"
#include "State.h"
#include "Game.h"

MainMenu::MainMenu()
{
}

void MainMenu::init()
{
	if (State::IsRunning())
		INDEX_RESUME = AddMenuItem("Resume");

	INDEX_NEWGAME = AddMenuItem("New Game");
	INDEX_EXIT = AddMenuItem("Exit");
}

void MainMenu::SelectEntry()
{
	if (menuIndex == INDEX_NEWGAME)
	{
		State::Set(Transition::Push, new Game);
	}
	else if (menuIndex == INDEX_RESUME)
	{
		State::Set(Transition::Pop);
	}
	else if (menuIndex == INDEX_EXIT)
	{
		State::Set(Transition::Exit);
	}
}
