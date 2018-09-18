#include "stdafx.h"
#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

void Inventory::AddItem(int gid, const std::string& itemName)
{
	GameItem *itemFound = nullptr;
	for (auto &item : items)
	{
		if (item.first->Name() == itemName)
		{
			itemFound = item.first;
			item.second++;
			break;
		}
	}
	
	if (!itemFound)
		items.emplace_back(std::make_pair(new GameItem(gid, itemName), 1));
}
