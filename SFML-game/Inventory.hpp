#pragma once
#include "GameItem.hpp"
#include "ObjectSprite.hpp"

class Inventory
{
public:
	Inventory() = default;
	~Inventory() = default;

	void takeItem(std::unique_ptr<GameItem> item);
	const auto &Items() const { return m_items; }

	auto size() const { return m_items.size(); }
	

	template <typename Self>
	auto& operator[](this Self&& self, size_t index)
	{
		return self.m_items[index];
	}

private:
	std::vector<std::unique_ptr<GameItem>> m_items;
};

