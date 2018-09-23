#pragma once
#include "PopupMenu.h"
class GamePopupMenu : public PopupMenu
{
public:
	GamePopupMenu();
	~GamePopupMenu() = default;
	void init() override;
};

