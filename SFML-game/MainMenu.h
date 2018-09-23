#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu() = default;
	~MainMenu() = default;
	void init() override;
};
