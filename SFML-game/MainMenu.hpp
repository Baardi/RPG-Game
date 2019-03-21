#pragma once
#include "Menu.hpp"

class MainMenu : public Menu
{
public:
	MainMenu() = default;
	~MainMenu() = default;
	void init() override;
};
