#include "stdafx.h"
#include "MainMenu.h"
#include "Machine.h"

MainMenu::MainMenu(sf::RenderWindow &window, sf::Event &event, sf::Font &font) : Menu(window, event, font)
{
	if (Machine::IsRunning())
		INDEX_RESUME = AddMenuItem("Resume");

	INDEX_NEWGAME = AddMenuItem("New Game");
	INDEX_EXIT = AddMenuItem("Exit");
}

void MainMenu::SelectEntry()
{
	if (menuIndex == INDEX_NEWGAME)
	{
		Machine::Set(Transition::Switch, State::Game);
	}
	else if (menuIndex == INDEX_RESUME)
	{
		Machine::Set(Transition::Pop);
	}
	else if (menuIndex == INDEX_EXIT)
	{
		Machine::Set(Transition::Exit);
	}
}
