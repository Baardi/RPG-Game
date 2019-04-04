#pragma once
#include "PopupMenu.hpp"

namespace ui {

class DialogInterface : public ui::PopupMenu
{
public:
	DialogInterface();
	~DialogInterface();
	void init() override;
};

}
