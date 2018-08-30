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
		AddMenuItem("Resume", &State::Reset<MainGame>);

	AddMenuItem("New Game", &State::Pop);
}
