#include "stdafx.h"
#include "ItemInfoPopup.hpp"
#include "Equipment.hpp"

void ItemInfoPopup::init()
{
	m_menuBackground.load("data/Menus/Subpopup.json", State::Textures());
	auto subInitializer = State::GetInitializer<ItemInfoInitializer>();
	if (!subInitializer)
		throw;
	
	x = subInitializer->x;
	y = subInitializer->y;
	m_renderTexture.create(352, 352);
	m_renderSprite.setPosition(x - 20, y - 20);
	m_renderSprite.setTexture(m_renderTexture.getTexture());

	m_item = subInitializer->item;
	addMenuItem("Back", State::Pop);

	auto equipment = dynamic_cast<Equipment *>(m_item);
	if (equipment)
	{
		auto &stats = equipment->getStats();
		addMenuItem("Attack:  " + std::to_string(stats.Attack));
		addMenuItem("Defence: " + std::to_string(stats.Defence)); 
		addMenuItem("HP:      " + std::to_string(stats.HP));
	}
}

void ItemInfoPopup::draw()
{
	m_renderTexture.clear(sf::Color::Transparent);
	m_menuBackground.draw(m_renderTexture);
	m_renderTexture.display();
	window.draw(m_renderSprite);
	
	Menu::draw();
}
