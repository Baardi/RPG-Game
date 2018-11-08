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
	menuBackground.load("data/Menus/PopupMenu.json", State::Textures());
}

InventoryUI::~InventoryUI()
{
}

void InventoryUI::init()
{
	AddMenuItem("Back", State::Pop);

	auto inventoryInitializer = State::GetInitializer<InventoryInitializer>();
	if (!inventoryInitializer)
		throw;

	inventory = &inventoryInitializer->inventory;
	size_t lastIndex = 0;
	for (auto &item : inventory->Items())
	{
		auto text = item.first->name() + "  x" + std::to_string(item.second);

		lastIndex = 
		AddMenuItem(text, item.first->sprite(), [this, &item, lastIndex]()
		{
			State::PushChild<ItemInfoPopup>();
			auto pos = GetMenuCoords(lastIndex + 1);
			State::SetInitializer<ItemInfoInitializer>(item.first.get(), pos.first + 380, pos.second);
		});
	}
}
