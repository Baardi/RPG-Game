#include "stdafx.h"
#include "GamePopupMenu.hpp"
#include "App/Ui/StateMachine.hpp"
#include "App/ResourceManager.hpp"
#include "Game.hpp"

using ui::GamePopupMenu;

GamePopupMenu::GamePopupMenu()
{
	x = 70;
	y = 120;
}

void GamePopupMenu::init()
{
	m_menuBackground.load("data/Menus/PopupMenu.json", resources().textures());

	if (stateMachine().isRunning())
		addMenuItem("Resume", [this] { stateMachine().popState(); });

	addMenuItem("New Game", [this] { stateMachine().reset<Game>(); });
}
