#include "stdafx.h"
#include "PopupMenu.h"


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
