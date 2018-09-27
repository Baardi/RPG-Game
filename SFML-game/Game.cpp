#include "stdafx.h"
#include "Game.h"
#include "MainMenu.h"

bool Game::frame()
{
	if (!UI::frame())
		return 	false;

	if (!pausable && !keyMapper.isControlKeyPressed())
		pausable = true;

	keyMapper.HandleKeyInput();
	return !paused;
}

void Game::init()
{
	UI::init();
}

void Game::pause()
{
	UI::pause();
}

void Game::resume()
{
	UI::resume();
	pausable = false;
}

void Game::toggle()
{
	if (!pausable)
		return;

	paused ? resume() : pause();
	pausable = false;
}
