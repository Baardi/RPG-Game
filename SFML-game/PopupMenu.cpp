#include "stdafx.h"
#include "PopupMenu.h"


PopupMenu::PopupMenu()
{
	x = 70;
	y = 120;
	map = new Map;
	map->load("data/PopupMenu.json");
}

PopupMenu::~PopupMenu()
{
	delete map;
}

void PopupMenu::draw()
{
	map->draw(window);
	Menu::draw();
}
