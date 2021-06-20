#include "stdafx.h"
#include "Stats.hpp"

Stats Stats::get(const MapProperties &properties)
{
	return Stats 
	{
		.HP		  = properties.getProperty<int>("HP"	  ).value_or(0),
		.Attack	  = properties.getProperty<int>("Attack"  ).value_or(0),
		.Defence  = properties.getProperty<int>("Defence" ).value_or(0),
		.Damage	  = properties.getProperty<int>("Damage"  ).value_or(0)
	};
}

Stats Stats::operator+(const Stats &other) const
{
	Stats stats;

	stats.HP		= HP		+ other.HP		;
	stats.Attack	= Attack	+ other.Attack	;
	stats.Defence	= Defence	+ other.Defence	;
	stats.Damage	= Damage	+ other.Damage	;

	return stats;
}

Stats Stats::operator-() const
{
	Stats stats;

	stats.HP		= -HP	  ;
	stats.Attack	= -Attack ;
	stats.Defence	= -Defence;
	stats.Damage	= -Damage ;

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

std::string Stats::toString() const
{
	std::stringstream sstream;

	sstream << "HP"			<< HP		<<	'\n';
	sstream << "Attack"		<< Attack	<<	'\n';
	sstream << "Defence"	<< Defence	<<	'\n';
	sstream << "Damage"		<< Damage	<<	'\n';

	return sstream.str();
}
