#pragma once
#include "GameItem.h"
#include "ObjectSprite.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	void AddItem(ObjectSprite *sprite);
	auto Items() const { return items; }

private:
	std::vector<std::pair<GameItem *, int>> items;
};

