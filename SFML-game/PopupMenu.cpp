#include "stdafx.h"
#include "PopupMenu.hpp"


PopupMenu::PopupMenu()
{
}

PopupMenu::~PopupMenu()
{
}

void PopupMenu::draw()
{
	m_menuBackground.draw(window);
	Menu::draw();
}
