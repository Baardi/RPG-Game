#include "stdafx.h"
#include "Inventory.h"

void Inventory::AddItem(ObjectSprite *sprite)
{
	auto item = std::unique_ptr<GameItem>(Object::create<GameItem>(sprite->type));
	if (!item)
		item = std::make_unique<GameItem>();

	item->construct(sprite->gid, sprite->name, sprite->sprite);
	item->applyProperties(*sprite);

	m_items.push_back(std::move(item));
}
