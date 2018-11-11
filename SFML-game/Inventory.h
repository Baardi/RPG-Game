#pragma once
#include "GameItem.h"
#include "ObjectSprite.h"

class Inventory
{
public:
	Inventory() = default;
	~Inventory() = default;

	void AddItem(ObjectSprite *sprite);
	const auto &Items() const { return m_items; }

private:
	std::vector<std::pair<std::unique_ptr<GameItem>, int>> m_items;
};

