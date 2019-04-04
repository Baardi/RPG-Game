#include "stdafx.h"
#include "MainMenu.hpp"
#include "App/Ui/StateMachine.hpp"
#include "Game.hpp"

using ui::MainMenu;

void MainMenu::init()
{
	if (stateMachine().isRunning())
		addMenuItem("Resume", [this] { stateMachine().popState(); });

	addMenuItem("New Game", [this] { stateMachine().reset<Game>(); });
	addMenuItem("Exit", [this] { stateMachine().exit(); });
}
