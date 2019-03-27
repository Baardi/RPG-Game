#include "stdafx.h"
#include "Weapon.hpp"

Weapon::Weapon(int gid, const std::string &name, const sf::Sprite &sprite) :
	Equipment(gid, name, sprite)
{}
