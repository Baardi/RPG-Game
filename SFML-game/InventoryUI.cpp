#include "stdafx.h"
#include "InventoryUI.hpp"
#include "MainMenu.hpp"
#include "Inventory.hpp"
#include "ItemInfoPopup.hpp"
#include "App/Ui/StateMachine.hpp"
#include "App/ResourceManager.hpp"

using ui::InventoryPopup;

InventoryPopup::InventoryPopup(Inventory &inventory) : inventory(&inventory)
{
	x = 70;
	y = 120;
	m_menuBackground.load("data/Menus/PopupMenu.json", resources().textures());
}

InventoryPopup::~InventoryPopup()
{
}

void InventoryPopup::init()
{
	addMenuItem("Back", [this] { stateMachine().popState(); });

	for (auto &item : inventory->Items())
	{
		auto &button = addMenuItem(item->name(), item->sprite());
		auto buttonPos = button.getPosition();
		auto pItem = item.get();

		button.setActionHandler([this, pItem, buttonPos]
		{
			stateMachine().pushChild<ItemInfoPopup>(pItem, buttonPos.x + 380, buttonPos.y);
		});
	}
}
