#pragma once
#include "GameItem.hpp"
class Valuable : public GameItem
{
public:
	Valuable();
	~Valuable();

	void applyProperties(const MapProperties &properties) {} // Dummy implementation
};

