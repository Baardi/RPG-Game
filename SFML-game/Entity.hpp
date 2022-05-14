#pragma once
#include "GameObject.hpp"
#include "Stats.hpp"

class Entity
{
public:
	Entity() 
	{
		m_fightTimer.reset(true);
	}
	virtual ~Entity() = default;

	virtual void fight(Entity &other) 
	{
		hit(other, m_stats.Damage);
	}

	void hit(Entity &other, int damage)
	{
		// TODO: 
		if (m_fightTimer.getElapsedTime() > sf::seconds(1.0f))
		{
			other.m_stats.HP -= damage;
			m_fightTimer.reset(true);
		}
	}

	bool isDead()
	{
		return m_stats.HP <= 0;
	}

	// Don't mess with this stuff during overrride
	sftools::Chronometer m_fightTimer;
	Stats m_stats;
};

