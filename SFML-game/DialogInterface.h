#pragma once
#include "PopupMenu.h"

class DialogInterface : public Menu
{
public:
	DialogInterface();
	~DialogInterface();
	void init() override;

private:
	Map menuBackground;
};

