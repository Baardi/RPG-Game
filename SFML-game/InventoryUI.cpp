#include "stdafx.h"
#include "InventoryUI.h"
#include "State.h"
#include "MainMenu.h"
#include "Inventory.h"


InventoryUI::InventoryUI()
{
	x = 70;
	y = 120;
	menuBackground.load("data/Menus/Inventory.json", State::Textures());
}

InventoryUI::~InventoryUI()
{
}

void InventoryUI::AddMenuSprite(const sf::Sprite& sprite)
{
	menusprites.emplace_back(sprite); // Yes I want a copy
	menusprites.back().setPosition(x + 220, y + spacing * menusprites.size());
}

void InventoryUI::init()
{
	AddMenuItem("Back", State::Pop);

	auto inventoryInitializer = State::GetInitializer<InventoryInitializer>();
	if (!inventoryInitializer)
		throw;

	inventory = &inventoryInitializer->inventory;
	for (auto item : inventory->Items())
	{
		AddMenuItem(item.first->name() + "  x" + std::to_string(item.second));
		AddMenuSprite(item.first->Sprite());
	}
}

void InventoryUI::draw()
{
	PopupMenu::draw();
	for (auto &item : menusprites)
		window.draw(item);
}
