#pragma once
#include "PopupMenu.hpp"
#include "GameItem.hpp"

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

	GameItem *m_item;
	Map m_menuBackground;
	sf::RenderTexture m_renderTexture;
	sf::Sprite m_renderSprite;
};

