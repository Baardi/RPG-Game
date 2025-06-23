#include "stdafx.h"
#include "ItemInfoPopup.hpp"
#include "Equipment.hpp"
#include "App/StateMachine.hpp"
#include "App/ResourceManager.hpp"

using appstate::ItemInfoPopup;

void ItemInfoPopup::init()
{
	m_menuBackground.load("data/Menus/Subpopup.json", resources().textures());
	m_renderSprite.setPosition(static_cast<sf::Vector2f>(pos - sf::Vector2i{ 20, 20 }));

	addMenuItem("Back", [this] { stateMachine().popState(); });

	auto equipment = dynamic_cast<Equipment *>(m_item);
	if (equipment)
	{
		auto &stats = equipment->getStats();
		addMenuItem("Attack:  " + std::to_string(stats.Attack));
		addMenuItem("Defence: " + std::to_string(stats.Defence)); 
		addMenuItem("HP:      " + std::to_string(stats.HP));
	}
}

void ItemInfoPopup::draw(sf::RenderTarget &target)
{
	m_renderTexture.clear(sf::Color::Transparent);
	m_menuBackground.draw(m_renderTexture);
	m_renderTexture.display();

	target.draw(m_renderSprite);
	
	Menu::draw(target);
}
