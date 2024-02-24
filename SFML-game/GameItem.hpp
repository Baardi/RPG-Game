#pragma once
#include "ObjectFactory.hpp"

class MapProperties;

class GameItem
{
public:
	GameItem() = default;
	GameItem(int gid, const std::string &name, const std::optional<sf::Sprite> &sprite);
	virtual ~GameItem() = default;

	void construct(int gid, const std::string &name, const std::optional<sf::Sprite> &sprite);
	virtual void applyProperties([[maybe_unused]]const MapProperties &properties) {}

	const std::string& name() const { return m_name; }
	int gid() const { return m_gid; }
	const sf::Sprite *sprite() const { return m_sprite.has_value() ? &m_sprite.value() : nullptr; }
	
private:
	std::optional<sf::Sprite> m_sprite;
	int m_gid;
	std::string m_name;
};

using GameItemFactory = ObjectFactory<GameItem>;
