#include "stdafx.h"
#include "Inventory.h"

void Inventory::AddItem(ObjectSprite *sprite)
{
	GameItem *itemFound = nullptr;
	for (auto &item : items)
	{
		if (item.first->name() == sprite->name)
		{
			itemFound = item.first.get();
			item.second++;
			break;
		}
	}
	
	if (!itemFound)
	{
		auto item = Object::create<GameItem>(sprite->type);
		item->vSet(sprite->gid, sprite->name, sprite->sprite);
		item->applyProperties(*sprite);
		items.emplace_back(item, 1);
	}
}
