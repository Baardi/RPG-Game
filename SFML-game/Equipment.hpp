#pragma once
#include "GameItem.h"
#include "StatsInterface.hpp"

class Equipment : public GameItem
{
public:
	Equipment() = default;
	Equipment(int gid, const std::string &name, const sf::Sprite &sprite);
	virtual ~Equipment() = default;

	void applyProperties(const MapProperties &properties) override;

protected:
	Stats m_stats;
};

