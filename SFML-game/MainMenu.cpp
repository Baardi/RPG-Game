#include "stdafx.h"
#include "MainMenu.h"
#include "State.h"
#include "MainGame.h"

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

void MainMenu::SelectEntry() const
{
	if (menuIndex == INDEX_NEWGAME)
	{
		State::Reset<MainGame>();
	}
	else if (menuIndex == INDEX_RESUME)
	{
		State::Pop();
	}
	else if (menuIndex == INDEX_EXIT)
	{
		State::Exit();
	}
}
