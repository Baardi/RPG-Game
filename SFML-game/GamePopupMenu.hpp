#pragma once
#include "PopupMenu.hpp"

namespace appstate {

class GamePopupMenu : public appstate::PopupMenu
{
public:
	GamePopupMenu();
	~GamePopupMenu() = default;
	void init() override;
};

}
