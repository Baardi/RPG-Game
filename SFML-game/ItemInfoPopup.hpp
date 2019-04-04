#pragma once
#include "PopupMenu.hpp"
#include "GameItem.hpp"

namespace ui {

class ItemInfoPopup : public ui::PopupMenu
{
public:
	ItemInfoPopup(GameItem *item, int x, int y) :
		m_item(item), x(x), y(y) {}

	virtual ~ItemInfoPopup() = default;

	void init() override;
	void draw(sf::RenderTarget &target) override; 
	
	GameItem *m_item;
	int x, y;
	Map m_menuBackground;
	sf::RenderTexture m_renderTexture;
	sf::Sprite m_renderSprite;
};

}
