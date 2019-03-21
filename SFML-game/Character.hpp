#pragma once
#include "GameObject.hpp"
#include "StatsInterface.hpp"

class Character : public GameObject, public StatsInterface
{
public:
	Character();
	virtual ~Character();
};

