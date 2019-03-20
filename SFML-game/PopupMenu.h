#pragma once
#include "Menu.h"
#include "map.h"

class PopupMenu : public Menu
{
public:
	PopupMenu();
	virtual ~PopupMenu();

	void draw() override; 

	Map m_menuBackground;
};

