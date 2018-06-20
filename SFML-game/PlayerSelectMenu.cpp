#include "stdafx.h"
#include "PlayerSelectMenu.h"
#include "Machine.h"
#include "Game.h"

PlayerSelectMenu::PlayerSelectMenu(sf::RenderWindow &window, sf::Event &event, sf::Font &font) : Menu(window, event, font)
{
	INDEX_1PLAYER = AddMenuItem("1 player");
	INDEX_2PLAYER = AddMenuItem("2 player");
	INDEX_3PLAYER = AddMenuItem("3 player");
	INDEX_4PLAYER = AddMenuItem("4 player");
	INDEX_BACK	  = AddMenuItem("Back"    );
}


void PlayerSelectMenu::SelectEntry()
{
	if (menuIndex == INDEX_1PLAYER)
	{
		Machine::Set(Transition::Reset, State::Game);
		Machine::SetInitializer(new GameInitializer(1));
	}
	else if (menuIndex == INDEX_2PLAYER)
	{
		Machine::Set(Transition::Reset, State::Game);
		Machine::SetInitializer(new GameInitializer(2));
	}
	else if (menuIndex == INDEX_3PLAYER)
	{
		Machine::Set(Transition::Reset, State::Game);
		Machine::SetInitializer(new GameInitializer(3));
	}
	else if (menuIndex == INDEX_4PLAYER)
	{
		Machine::Set(Transition::Reset, State::Game); 
		Machine::SetInitializer(new GameInitializer(4));
	}
	else if (menuIndex == INDEX_BACK)
	{
		Machine::Set(Transition::Pop);
	}
}
