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
	menuBackground.draw(window);
	Menu::draw();
}
