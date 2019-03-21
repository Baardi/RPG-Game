#pragma once
#include "PopupMenu.hpp"

class DialogInterface : public PopupMenu
{
public:
	DialogInterface();
	~DialogInterface();
	void init() override;
};

