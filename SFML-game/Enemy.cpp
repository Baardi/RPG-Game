#include "stdafx.h"
#include "Enemy.hpp"

#include <print>

void Enemy::applyProperties()
{
	const std::string test = "test:{}";
	std::println(std::runtime_format(test), 1);

	m_stats = Stats::get(*this);
}
