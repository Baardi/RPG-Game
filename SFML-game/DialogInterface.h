#pragma once
#include "PopupMenu.h"

class DialogInterface : public PopupMenu
{
public:
	DialogInterface();
	~DialogInterface();
	void init() override;
};

