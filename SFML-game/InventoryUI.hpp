#pragma once
#include "PopupMenu.hpp"
#include "Inventory.hpp"

namespace appstate {

class InventoryPopup : public appstate::PopupMenu
{
public:
	InventoryPopup(Inventory &inventory);
	~InventoryPopup();
	void init() override;

private:
	Inventory *inventory;
};

}