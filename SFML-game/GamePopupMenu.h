#pragma once
#include "PopupMenu.h"
class GamePopupMenu : public PopupMenu
{
public:
	GamePopupMenu() = default;
	~GamePopupMenu() = default;
	void init() override;

private:
	size_t INDEX_NEWGAME = -1;
	size_t INDEX_RESUME = -1;

	void SelectEntry() const override;
};

