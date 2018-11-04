#pragma once

struct Stats
{
	int HP;
	int Attack;
	int Defense;
};

class StatsInterface
{
public:
	StatsInterface();
	virtual ~StatsInterface();
	Stats m_stats;
};

