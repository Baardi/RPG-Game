#include "stdafx.h"
#include "GamePopupMenu.h"
#include "State.h"
#include "MainGame.h"

GamePopupMenu::GamePopupMenu()
{
	x = 70;
	y = 120;
}

void GamePopupMenu::init()
{
	menuBackground.load("data/Maps/PopupMenu.json", State::Textures());

	if (State::IsRunning())
		AddMenuItem("Resume", State::Pop);

	AddMenuItem("New Game", State::Reset<MainGame>);
}
