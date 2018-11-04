#pragma once
#include "Equipment.hpp"

class Weapon : public Equipment
{
public:
	Weapon() = default;
	Weapon(int gid, const std::string &name, const sf::Sprite &sprite);
	~Weapon() = default;
};

