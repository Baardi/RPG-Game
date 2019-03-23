#pragma once
#include "Menu.hpp"
#include "Map.hpp"

class PopupMenu : public Menu
{
public:
	PopupMenu();
	virtual ~PopupMenu();

	void draw(sf::RenderTarget &target) override;

	Map m_menuBackground;
};

