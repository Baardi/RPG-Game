#include "stdafx.h"
#include "GameItem.h"


GameItem::GameItem(int gid, const std::string &name)
{
	m_name = name;
	m_gid = gid;
}

GameItem::~GameItem()
{
}

void GameItem::SetSprite(sf::Sprite &sprite)
{
	m_sprite = sprite;
}
