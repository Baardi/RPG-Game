#pragma once
#include "PopupMenu.hpp"

namespace ui {

class GamePopupMenu : public ui::PopupMenu
{
public:
	GamePopupMenu();
	~GamePopupMenu() = default;
	void init() override;
};

}
