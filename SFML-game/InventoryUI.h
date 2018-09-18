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
	void init() override;
};
