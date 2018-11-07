#include "stdafx.h"
#include "Stats.hpp"

Stats Stats::get(const MapProperties &properties)
{
	Stats stats;

	properties.GetProperty("Attack"	,	&stats.Attack	);
	properties.GetProperty("Defence",   &stats.Defence	);
	properties.GetProperty("HP"		,	&stats.HP 		);

	return stats;
}

Stats Stats::operator+(const Stats &other) const
{
	Stats stats;

	stats.Attack  =	Attack	+ other.Attack	;
	stats.Defence = Defence + other.Defence	;
	stats.HP      =	HP		+ other.HP		;

	return stats;
}

Stats Stats::operator-() const
{
	Stats stats;

	stats.Attack	= -Attack	;
	stats.Defence	= -Defence	;
	stats.HP		= -HP		;

	return stats;
}

Stats Stats::operator+() const
{
	return *this;
}

Stats &Stats::operator+=(const Stats &other)
{
	*this = (*this) + other;
	return *this;
}

Stats Stats::operator-(const Stats &other) const
{
	return (*this) + (-other);
}

Stats &Stats::operator-=(const Stats &other)
{
	*this = (*this) - other;
	return *this;
}
