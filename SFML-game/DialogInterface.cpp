#include "stdafx.h"
#include "DialogInterface.hpp"
#include "State.hpp"


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
	m_menuBackground.load("data/Menus/MessageBox.json", State::Textures());

	addMenuItem("This is a dummy message", State::Pop);
}
