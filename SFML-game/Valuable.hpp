#pragma once
#include "GameItem.hpp"
class Valuable : public GameItem
{
public:
	Valuable();
	~Valuable();

	void applyProperties([[maybe_unused]]const MapProperties &properties) {} // Dummy implementation
};

