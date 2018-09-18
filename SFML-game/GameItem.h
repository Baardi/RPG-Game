#pragma once

#include "TileLayer.h"
class GameItem
{
public:
	GameItem(int gid, const std::string &name);
	~GameItem();

	void SetSprite(sf::Sprite &sprite);
	const std::string& name() const { return m_name; }
	int gid() const { return m_gid; }
	const sf::Sprite const &Sprite() const { return m_sprite; }
	
private:
	sf::Sprite m_sprite;
	int m_gid;
	std::string m_name;
};

