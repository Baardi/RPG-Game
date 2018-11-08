#include "stdafx.h"
#include "DialogInterface.h"
#include "State.h"


DialogInterface::DialogInterface()
{
}


DialogInterface::~DialogInterface()
{
}

void DialogInterface::init()
{
	x = 240;
	y = 780;
	colorSelect = sf::Color::Blue;
	colorUnselect = sf::Color::Black;
	menuBackground.load("data/Menus/MessageBox.json", State::Textures());

	AddMenuItem("This is a dummy message", State::Pop);
}
