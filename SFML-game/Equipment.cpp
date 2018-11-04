#include "stdafx.h"
#include "Equipment.hpp"

Equipment::Equipment(int gid, const std::string & name, const sf::Sprite &sprite) :
	GameItem(gid, name, sprite)
{}
