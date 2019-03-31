#pragma once
#include "PopupMenu.hpp"
#include "Inventory.hpp"

class InventoryUI : public PopupMenu
{
public:
	InventoryUI(Inventory &inventory);
	~InventoryUI();
	void init() override;

private:
	Inventory *inventory;
};
