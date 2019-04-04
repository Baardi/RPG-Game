#include "stdafx.h"
#include "DialogInterface.hpp"
#include "App/Ui/StateMachine.hpp"
#include "App/ResourceManager.hpp"

using ui::DialogInterface;

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
	m_menuBackground.load("data/Menus/MessageBox.json", resources().textures());

	addMenuItem("This is a dummy message", [this] { stateMachine().popState(); });
}
