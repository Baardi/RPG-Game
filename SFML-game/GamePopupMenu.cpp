#include "stdafx.h"
#include "GamePopupMenu.hpp"
#include "App/StateMachine.hpp"
#include "App/ResourceManager.hpp"
#include "Game.hpp"

using appstate::GamePopupMenu;

GamePopupMenu::GamePopupMenu()
{
	pos = { 70, 120 };
}

void GamePopupMenu::init()
{
	m_menuBackground.load("data/Menus/PopupMenu.json", resources().textures());

	if (stateMachine().isRunning())
		addMenuItem("Resume", [this] { stateMachine().popState(); });

	addMenuItem("New Game", [this] { stateMachine().reset<Game>(); });
}
