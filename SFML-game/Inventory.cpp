#include "stdafx.h"
#include "Inventory.hpp"

void Inventory::takeItem(std::unique_ptr<GameItem> &&item)
{
	m_items.push_back(std::move(item));
}
