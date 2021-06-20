#include "stdafx.h"
#include "InventoryUI.hpp"
#include "MainMenu.hpp"
#include "Inventory.hpp"
#include "ItemInfoPopup.hpp"
#include "App/StateMachine.hpp"
#include "App/ResourceManager.hpp"

using appstate::InventoryPopup;

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

		button.onClicked([this, item = item.get(), &button = button]
		{
			auto buttonPos = button.getPosition();
			stateMachine().pushChild<ItemInfoPopup>(item, static_cast<int>(buttonPos.x) + 380, static_cast<int>(buttonPos.y));
		});
	}
}
