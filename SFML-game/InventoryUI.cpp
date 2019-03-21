#include "stdafx.h"
#include "InventoryUI.hpp"
#include "State.hpp"
#include "MainMenu.hpp"
#include "Inventory.hpp"
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
	for (auto &item : inventory->Items())
	{
		auto &button = addMenuItem(item->name(), item->sprite());
		auto buttonPos = button.getPosition();
		auto pItem = item.get();

		button.setActionHandler([this, pItem, buttonPos]
		{
			State::PushChild<ItemInfoPopup>();
			State::SetInitializer<ItemInfoInitializer>(pItem, buttonPos.x + 380, buttonPos.y);
		});
	}
}
