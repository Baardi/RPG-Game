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

	auto size() const { return m_items.size(); }
	
	std::pair<GameItem *, int> &operator[](std::size_t idx) 
	{ 
		auto &pair = m_items[idx];
		return std::make_pair(pair.first.get(), pair.second);
	}

	const std::pair<const GameItem *, int> &operator[](std::size_t idx) const 
	{ 
		auto &pair = m_items[idx];
		return std::make_pair(pair.first.get(), pair.second);
	}

private:
	std::vector<std::pair<std::unique_ptr<GameItem>, int>> m_items;
};

