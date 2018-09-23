#include "stdafx.h"
#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

void Inventory::AddItem(ObjectSprite *sprite)
{
	GameItem *itemFound = nullptr;
	for (auto &item : items)
	{
		if (item.first->name() == sprite->name)
		{
			itemFound = item.first;
			item.second++;
			break;
		}
	}
	
	if (!itemFound)
	{
		auto pair = std::make_pair(new GameItem(sprite->gid, sprite->name), 1);
		pair.first->SetSprite(sprite->sprite);
		items.emplace_back(pair);
	}
}
