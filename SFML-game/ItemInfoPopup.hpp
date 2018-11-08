#pragma once
#include "PopupMenu.h"
#include "GameItem.h"

class ItemInfoInitializer : public Initializer
{
public:
	ItemInfoInitializer(GameItem *item, int x, int y) :
		item(item), x(x), y(y) {}

	GameItem *item;
	int x, y;
};

class ItemInfoPopup : public PopupMenu
{
public:
	ItemInfoPopup() = default;
	virtual ~ItemInfoPopup() = default;

	void init() override;
	void draw() override; 

	GameItem *item;
	Map menuBackground;
	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;
};

