#pragma once
#include "PopupMenu.hpp"
#include "State.hpp"
#include "Inventory.hpp"

class InventoryInitializer : public Initializer
{
public:
	InventoryInitializer(Inventory &inventory) : inventory(inventory) {}
	Inventory &inventory;
};

class InventoryUI : public PopupMenu
{
public:
	InventoryUI();
	~InventoryUI();
	void init() override;

private:
	Inventory *inventory;
};
