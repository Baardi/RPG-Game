#pragma once
#include "MapProperties.h"

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

	// Stats
	int HP		= 0;
	int Attack	= 0;
	int Defence = 0;
};
