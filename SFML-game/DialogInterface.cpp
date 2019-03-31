#include "stdafx.h"
#include "DialogInterface.hpp"
#include "StateHandler.hpp"
#include "ResourceHandler.hpp"

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
	m_menuBackground.load("data/Menus/MessageBox.json", resourceHandler().textures());

	addMenuItem("This is a dummy message", [this] { stateHandler().popState(); });
}
