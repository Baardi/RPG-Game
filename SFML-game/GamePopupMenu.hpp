#pragma once
#include "PopupMenu.hpp"
class GamePopupMenu : public PopupMenu
{
public:
	GamePopupMenu();
	~GamePopupMenu() = default;
	void init() override;
};

