#pragma once
#include "GameItem.hpp"
#include "ObjectSprite.hpp"

class Inventory
{
public:
	Inventory() = default;
	~Inventory() = default;

	void takeItem(std::unique_ptr<GameItem> &&item);
	const auto &Items() const { return m_items; }

	auto size() const { return m_items.size(); }
	
	auto &operator[](std::size_t idx) 
	{
		return m_items[idx];
	}

	const auto &operator[](std::size_t idx) const 
	{ 
		return m_items[idx];
	}

private:
	std::vector<std::unique_ptr<GameItem>> m_items;
};

