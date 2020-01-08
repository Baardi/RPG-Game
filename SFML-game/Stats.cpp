#include "stdafx.h"
#include "Stats.hpp"

Stats Stats::get(const MapProperties &properties)
{
	Stats stats;

	properties.getProperty("Attack"	,	&stats.Attack	);
	properties.getProperty("Defence",   &stats.Defence	);
	properties.getProperty("Damage" ,	&stats.Damage	);
	properties.getProperty("HP"		,	&stats.HP 		);

	return stats;
}

Stats Stats::operator+(const Stats &other) const
{
	Stats stats;

	stats.Attack	= Attack	+ other.Attack	;
	stats.Defence	= Defence	+ other.Defence	;
	stats.Damage	= Damage	+ other.Damage	;
	stats.HP		= HP		+ other.HP		;

	return stats;
}

Stats Stats::operator-() const
{
	Stats stats;

	stats.Attack	= -Attack	;
	stats.Defence	= -Defence	;
	stats.Damage	= -Damage	;
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

std::string Stats::toString()
{
	std::stringstream sstream;

	sstream << "Attack"  << Attack << '\n';
	sstream << "Defence" << Defence << '\n';
	sstream << "Damage"  << Damage << '\n';
	sstream << "HP"		 << HP;

	return sstream.str();
}
