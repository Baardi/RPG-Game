#include "stdafx.h"
#include "InventoryUI.h"
#include "State.h"
#include "MainMenu.h"
#include "Inventory.h"


InventoryUI::InventoryUI()
{
	x = 70;
	y = 120;
	menuBackground.load("data/PopupMenu.json");
}

InventoryUI::~InventoryUI()
{

}

void InventoryUI::init()
{
	AddMenuItem("Back", State::Pop);

	InventoryInitializer *inventoryInitializer = dynamic_cast<InventoryInitializer *>(State::GetInitializer());
	if (!inventoryInitializer)
		throw;

	Inventory *inventory = inventoryInitializer->inventory;
	for (auto item : inventory->Items())
	{
		AddMenuItem(item.first->Name() + "  x" + std::to_string(item.second), []() {});
	}
}
