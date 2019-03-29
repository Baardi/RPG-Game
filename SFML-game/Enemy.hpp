#pragma once
#include "ObjectSprite.hpp"
#include "Entity.hpp"

class Enemy : public ObjectSprite, public Entity
{
public:
	Enemy(const TileSize &tileSize) : ObjectSprite(tileSize) {}
	~Enemy() = default;

	void applyProperties() override;
};

