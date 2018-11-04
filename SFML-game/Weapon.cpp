#include "stdafx.h"
#include "Weapon.hpp"

REGISTER_TYPE(Weapon)

Weapon::Weapon(int gid, const std::string &name, const sf::Sprite &sprite) :
	Equipment(gid, name, sprite)
{}
