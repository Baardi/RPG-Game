#pragma once
#include "Menu.hpp"

namespace ui {

class MainMenu : public ui::Menu
{
public:
	MainMenu() = default;
	~MainMenu() = default;
	void init() override;
};

}