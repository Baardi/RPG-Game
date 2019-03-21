#pragma once
#include "GameItem.hpp"
#include "Stats.hpp"

class Equipment : public GameItem
{
public:
	Equipment() = default;
	Equipment(int gid, const std::string &name, const sf::Sprite &sprite);
	virtual ~Equipment() = default;

	const Stats &getStats() { return m_stats; }
	void applyProperties(const MapProperties &properties) override;

protected:
	Stats m_stats;
};

