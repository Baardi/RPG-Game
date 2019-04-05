#pragma once
#include "Menu.hpp"

namespace appstate {

class MainMenu : public appstate::Menu
{
public:
	MainMenu() = default;
	~MainMenu() = default;
	void init() override;
};

}