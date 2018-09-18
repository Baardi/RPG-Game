#pragma once
#include "GameItem.h"

class Inventory
{
public:
	Inventory();
	~Inventory();

	void AddItem(int gid, const std::string &itemName);
	std::vector<std::pair<GameItem *, int>> Items() const { return items; }

private:
	std::vector<std::pair<GameItem *, int>> items;
};

