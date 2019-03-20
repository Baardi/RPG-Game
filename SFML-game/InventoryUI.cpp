#include "stdafx.h"
#include "InventoryUI.h"
#include "State.h"
#include "MainMenu.h"
#include "Inventory.h"
#include "ItemInfoPopup.hpp"

InventoryUI::InventoryUI()
{
	x = 70;
	y = 120;
	m_menuBackground.load("data/Menus/PopupMenu.json", State::Textures());
}

InventoryUI::~InventoryUI()
{
}

void InventoryUI::init()
{
	addMenuItem("Back", State::Pop);

	auto inventoryInitializer = State::GetInitializer<InventoryInitializer>();
	if (!inventoryInitializer)
		return;

	inventory = &inventoryInitializer->inventory;
	size_t lastIndex = 0;
	for (auto &item : inventory->Items())
	{
		lastIndex = addMenuItem(item->name(), item->sprite(), [this, &item, lastIndex]()
		{
			auto [x, y] = GetMenuCoords(lastIndex + 1);
			
			State::PushChild<ItemInfoPopup>();
			State::SetInitializer<ItemInfoInitializer>(item.get(), x + 380, y);
		});
	}
}
