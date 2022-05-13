#pragma once
#include "PopupMenu.hpp"
#include "GameItem.hpp"

namespace appstate {

class ItemInfoPopup : public appstate::PopupMenu
{
public:
	ItemInfoPopup(GameItem *item, sf::Vector2i pos) :
		m_item(item), m_pos(pos) {}

	virtual ~ItemInfoPopup() = default;

	void init() override;
	void draw(sf::RenderTarget &target) override; 
	
	GameItem *m_item;
	sf::Vector2i m_pos;
	Map m_menuBackground;
	sf::RenderTexture m_renderTexture;
	sf::Sprite m_renderSprite;
};

}
