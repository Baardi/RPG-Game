#include "stdafx.h"
#include "GamePopupMenu.hpp"
#include "State.hpp"
#include "Game.hpp"

GamePopupMenu::GamePopupMenu()
{
	x = 70;
	y = 120;
}

void GamePopupMenu::init()
{
	m_menuBackground.load("data/Menus/PopupMenu.json", State::Textures());

	if (State::IsRunning())
		addMenuItem("Resume", State::Pop);

	addMenuItem("New Game", State::Reset<Game>);
}
