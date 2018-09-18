#include "stdafx.h"
#include "GameItem.h"


GameItem::GameItem(int gid, const std::string &name)
{
	this->name = name;
	this->gid = gid;
}


GameItem::~GameItem()
{
}
