#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu();
	~MainMenu() = default;
	void init() override;

private:
	size_t INDEX_NEWGAME = -1;
	size_t INDEX_RESUME = -1;
	size_t INDEX_EXIT = -1;

	void SelectEntry() override;
};
