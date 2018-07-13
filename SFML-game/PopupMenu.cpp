#include "stdafx.h"
#include "PopupMenu.h"


PopupMenu::PopupMenu()
{
}

PopupMenu::~PopupMenu()
{
	if (map)
		delete map;
}

void PopupMenu::draw()
{
	if (map)
		map->draw(window);

	Menu::draw();
}
