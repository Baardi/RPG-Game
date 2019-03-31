#include "stdafx.h"
#include "GamePopupMenu.hpp"
#include "StateHandler.hpp"
#include "ResourceHandler.hpp"
#include "Game.hpp"

GamePopupMenu::GamePopupMenu()
{
	x = 70;
	y = 120;
}

void GamePopupMenu::init()
{
	m_menuBackground.load("data/Menus/PopupMenu.json", resourceHandler().textures());

	if (stateHandler().isRunning())
		addMenuItem("Resume", [this] { stateHandler().popState(); });

	addMenuItem("New Game", [this] { stateHandler().reset<Game>(); });
}
