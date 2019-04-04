#include "stdafx.h"
#include "ResourceManager.hpp"

ResourceManager &resources()
{
	static ResourceManager resourceManager;
	return resourceManager;
}