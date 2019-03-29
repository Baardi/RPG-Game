#include "stdafx.h"
#include "Enemy.hpp"

void Enemy::applyProperties()
{
	m_stats = Stats::get(*this);
}
