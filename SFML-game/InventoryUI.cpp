#include "stdafx.h"
#include "InventoryUI.h"
#include "State.h"
#include "MainMenu.h"


InventoryUI::InventoryUI()
{
	x = 70;
	y = 120;
	menuBackground.load("data/PopupMenu.json");
	INDEX_EXITGAME = AddMenuItem("Hei");
}

InventoryUI::~InventoryUI()
{

}

void InventoryUI::SelectEntry() const
{
	if (menuIndex == INDEX_EXITGAME)
	{
		const_cast<Map&>(menuBackground).load("data/Intro village.json");
	}
}


