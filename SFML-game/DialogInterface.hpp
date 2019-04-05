#pragma once
#include "PopupMenu.hpp"

namespace appstate {

class DialogInterface : public appstate::PopupMenu
{
public:
	DialogInterface();
	~DialogInterface();
	void init() override;
};

}
