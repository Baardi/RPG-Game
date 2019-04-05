#pragma once
#include "Menu.hpp"
#include "Map.hpp"

namespace appstate {

class PopupMenu : public appstate::Menu
{
public:
	PopupMenu();
	virtual ~PopupMenu();

	void draw(sf::RenderTarget &target) override;

	Map m_menuBackground;
};

}