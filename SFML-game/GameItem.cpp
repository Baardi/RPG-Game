#include "stdafx.h"
#include "GameItem.h"


GameItem::GameItem(int gid, const std::string &name, const sf::Sprite &sprite) :
	m_name(name),
	m_gid(gid),
	m_sprite(sprite)
{}

void GameItem::construct(int gid, const std::string & name, const sf::Sprite &sprite) 
{ 
	m_gid = gid;
	m_name = name; 
	m_sprite = sprite;
}