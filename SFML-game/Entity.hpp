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
		if (m_fightTimer.getElapsedTime_s() > 1)
			hit(other, m_stats.Damage);

		if (other.m_fightTimer.getElapsedTime_s() > 1)
			other.hit(*this, other.m_stats.Damage);
	}

	void hit(Entity &other, int damage)
	{
		other.m_stats.HP -= damage;
		m_fightTimer.reset(true);
	}

	bool isDead()
	{
		return m_stats.HP <= 0;
	}

	sftools::Chronometer m_fightTimer;
	Stats m_stats;
};

