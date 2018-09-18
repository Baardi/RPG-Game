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
	x = 120;
	y = 800;
	colorSelect = sf::Color::Blue;
	colorUnselect = sf::Color::Black;
	menuBackground.load("data/messagebox");

	AddMenuItem("This is a dummy message", State::Pop);
}
