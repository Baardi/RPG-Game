#include "stdafx.h"
#include "MainMenu.hpp"
#include "StateHandler.hpp"
#include "Game.hpp"

void MainMenu::init()
{
	if (stateHandler().isRunning())
		addMenuItem("Resume", [this] { stateHandler().popState(); });

	addMenuItem("New Game", [this] { stateHandler().reset<Game>(); });
	addMenuItem("Exit", [this] { stateHandler().exit(); });
}
