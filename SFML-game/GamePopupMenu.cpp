#include "stdafx.h"
#include "GamePopupMenu.h"
#include "State.h"
#include "Game.h"

void GamePopupMenu::init()
{
	x = 70;
	y = 120;
	map = new Map;
	map->load("data/PopupMenu.json");

	if (State::IsRunning())
		INDEX_RESUME = AddMenuItem("Resume");

	INDEX_NEWGAME = AddMenuItem("New Game");
}

void GamePopupMenu::SelectEntry() const
{
	if (menuIndex == INDEX_NEWGAME)
	{
		State::Set(Transition::Reset, new Game);
	}
	else if (menuIndex == INDEX_RESUME)
	{
		State::Set(Transition::Pop);
	}
}