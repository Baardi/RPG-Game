#include "stdafx.h"
#include "PopupMenu.hpp"

using ui::PopupMenu;

PopupMenu::PopupMenu()
{
}

PopupMenu::~PopupMenu()
{
}

void PopupMenu::draw(sf::RenderTarget &target)
{
	m_menuBackground.draw(target);
	Menu::draw(target);
}
