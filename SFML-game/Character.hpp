#pragma once
#include "GameObject.h"
#include "StatsInterface.hpp"

class Character : public GameObject, public StatsInterface
{
public:
	Character();
	virtual ~Character();
};

