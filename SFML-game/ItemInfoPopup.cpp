#include "stdafx.h"
#include "ItemInfoPopup.hpp"
#include "Equipment.hpp"

void ItemInfoPopup::init()
{
	menuBackground.load("data/Menus/Subpopup.json", State::Textures());
	auto subInitializer = State::GetInitializer<ItemInfoInitializer>();
	if (!subInitializer)
		throw;
	
	x = subInitializer->x;
	y = subInitializer->y;
	renderTexture.create(352, 352);
	renderSprite.setPosition(x - 20, y - 20);
	renderSprite.setTexture(renderTexture.getTexture());

	item = subInitializer->item;
	AddMenuItem("Back", State::Pop);

	auto equipment = dynamic_cast<Equipment *>(item);
	if (equipment)
	{
		auto &stats = equipment->getStats();
		AddMenuItem("Attack:  " + std::to_string(stats.Attack));
		AddMenuItem("Defence: " + std::to_string(stats.Defence)); 
		AddMenuItem("HP:      " + std::to_string(stats.HP));
	}
}

void ItemInfoPopup::draw()
{
	renderTexture.clear(sf::Color::Transparent);
	menuBackground.draw(renderTexture);
	renderTexture.display();
	window.draw(renderSprite);
	
	Menu::draw();
}
