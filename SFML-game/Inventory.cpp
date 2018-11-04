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
		GameItem *item = MAKE_INSTANCE(sprite->type, GameItem);
		if (item)
		{
			item->vSet(sprite->gid, sprite->name, sprite->sprite);
			items.emplace_back(item, 1);
		}
	}
}
