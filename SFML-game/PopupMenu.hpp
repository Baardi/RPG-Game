#pragma once
#include "Menu.hpp"
#include "Map.hpp"

namespace ui {

class PopupMenu : public ui::Menu
{
public:
	PopupMenu();
	virtual ~PopupMenu();

	void draw(sf::RenderTarget &target) override;

	Map m_menuBackground;
};

}