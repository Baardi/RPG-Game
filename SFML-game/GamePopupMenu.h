#pragma once
#include "PopupMenu.h"
class GamePopupMenu : public PopupMenu
{
public:
	GamePopupMenu() = default;
	~GamePopupMenu() = default;
	void init() override;
};

