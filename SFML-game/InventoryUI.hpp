#pragma once
#include "PopupMenu.hpp"
#include "Inventory.hpp"

namespace ui {

class InventoryPopup : public ui::PopupMenu
{
public:
	InventoryPopup(Inventory &inventory);
	~InventoryPopup();
	void init() override;

private:
	Inventory *inventory;
};

}