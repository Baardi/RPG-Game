#pragma once
#include "GameItem.hpp"
#include "ObjectSprite.hpp"
#include "InheritCVREF.hpp"

class Inventory
{
public:
	Inventory() = default;
	~Inventory() = default;

	void takeItem(std::unique_ptr<GameItem> item);
	const auto &Items() const { return m_items; }

	auto size() const { return m_items.size(); }
	
	template <typename Self>
		requires (std::is_lvalue_reference_v<Self&&> && !std::is_volatile_v<std::remove_reference_t<Self>>)
	auto &&operator[](this Self&& self, size_t index)
	{
		return inherit_qualifiers<Self&&>(*self.m_items[index]);
	}

private:
	std::vector<std::unique_ptr<GameItem>> m_items;
};
