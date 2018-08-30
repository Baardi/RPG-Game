#include "stdafx.h"
#include "GamePopupMenu.h"
#include "State.h"
#include "MainGame.h"

void GamePopupMenu::init()
{
	x = 70;
	y = 120;
	menuBackground.load("data/PopupMenu.json");

	if (State::IsRunning())
		INDEX_RESUME = AddMenuItem("Resume");

	INDEX_NEWGAME = AddMenuItem("New Game");
}

void GamePopupMenu::SelectEntry() const
{
	if (menuIndex == INDEX_NEWGAME)
	{
		State::Reset<MainGame>();
	}
	else if (menuIndex == INDEX_RESUME)
	{
		State::Pop();
	}
}