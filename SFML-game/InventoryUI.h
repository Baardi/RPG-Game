#pragma once
#include "PopupMenu.h"
#include "State.h"
#include "Inventory.h"

class InventoryInitializer : public Initializer
{
public:
	Inventory *inventory;
};

class InventoryUI : public PopupMenu
{
public:
	InventoryUI();
	~InventoryUI();
	void AddMenuSprite(const sf::Sprite &sprite);
	void init() override;
	void draw() override;

private:
	std::vector<sf::Sprite> menusprites;
	Inventory *inventory;
};
