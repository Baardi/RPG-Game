#include "stdafx.h"
#include "InventoryUI.hpp"
#include "MainMenu.hpp"
#include "Inventory.hpp"
#include "ItemInfoPopup.hpp"
#include "StateHandler.hpp"
#include "ResourceHandler.hpp"

InventoryUI::InventoryUI(Inventory &inventory) : inventory(&inventory)
{
	x = 70;
	y = 120;
	m_menuBackground.load("data/Menus/PopupMenu.json", resourceHandler().textures());
}

InventoryUI::~InventoryUI()
{
}

void InventoryUI::init()
{
	addMenuItem("Back", [this] { stateHandler().popState(); });

	for (auto &item : inventory->Items())
	{
		auto &button = addMenuItem(item->name(), item->sprite());
		auto buttonPos = button.getPosition();
		auto pItem = item.get();

		button.setActionHandler([this, pItem, buttonPos]
		{
			stateHandler().pushChild<ItemInfoPopup>(pItem, buttonPos.x + 380, buttonPos.y);
		});
	}
}
