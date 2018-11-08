#pragma once
#include "TileLayer.h"
#include "TypeMapper.h"

class GameItem : public Object
{
public:
	GameItem() = default;
	GameItem(int gid, const std::string &name, const sf::Sprite &sprite);
	virtual ~GameItem() = default;

	void vSet(int gid, const std::string &name, const sf::Sprite &sprite);
	virtual void applyProperties(const MapProperties &properties) {}

	const std::string& name() const { return m_name; }
	int gid() const { return m_gid; }
	const sf::Sprite &sprite() const { return m_sprite; }
	
private:
	sf::Sprite m_sprite;
	int m_gid;
	std::string m_name;
};
