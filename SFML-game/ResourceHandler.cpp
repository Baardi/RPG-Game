#include "stdafx.h"
#include "ResourceHandler.hpp"

ResourceHandler &resourceHandler()
{
	static ResourceHandler resources;
	return resources;
}