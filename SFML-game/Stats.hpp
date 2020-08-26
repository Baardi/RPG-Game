#pragma once
#include "MapProperties.hpp"

struct Stats
{
	static Stats get(const MapProperties &properties);
	
	// Overloads for arithmethics
	Stats operator+() const;
	Stats operator-() const;
	Stats operator+(const Stats &other) const;
	Stats operator-(const Stats &other) const;
	Stats &operator+=(const Stats &other);
	Stats &operator-=(const Stats &other);

	std::string toString() const;

	// Stats
	int HP		= 0;
	int Attack	= 0;
	int Defence = 0;
	int Damage	= 0;
};
