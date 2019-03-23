#include "stdafx.h"
#include "PopupMenu.hpp"


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
