#pragma once
#include "Stats.hpp"

class StatsInterface
{
public:
	StatsInterface() = default;
	virtual ~StatsInterface() = default;
	Stats m_stats;
};
