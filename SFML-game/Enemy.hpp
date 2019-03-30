#pragma once
#include "ObjectSprite.hpp"
#include "Entity.hpp"

class Enemy : public ObjectSprite, public Entity
{
public:
	Enemy() = default;
	~Enemy() = default;

	void applyProperties() override;
};

