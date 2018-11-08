#include "stdafx.h"
#include "ItemInfoPopup.hpp"

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

	item = subInitializer->item;
	AddMenuItem("Dummy", State::Pop);
}

void ItemInfoPopup::draw()
{
	renderTexture.clear(sf::Color::Transparent);
	menuBackground.draw(renderTexture);
	renderTexture.display();
	renderSprite.setTexture(renderTexture.getTexture());
	window.draw(renderSprite);
	
	Menu::draw();
}
