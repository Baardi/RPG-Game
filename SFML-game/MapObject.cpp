#include "stdafx.h"
#include "MapObject.h"
#include <SFML/Graphics.hpp>

void MapObject::getTileCoords(sf::Texture *texture, int tile, int& x, int& y)
{
	int tileXcount = texture->getSize().x / (tileSize.x + tileSize.s);

	x = (tile % tileXcount) * (tileSize.x + tileSize.s);
	y = (tile / tileXcount) * (tileSize.x + tileSize.s);
}

int MapObject::GetTextureIndex(int tileValue)
{
	int tileTextureValue = 0;

	for (auto iter = tileSets.begin(); iter != tileSets.end(); ++iter)
	{
		auto tmpKeyVal = iter->first;
		if (tmpKeyVal > tileTextureValue && tmpKeyVal < tileValue)
			tileTextureValue = tmpKeyVal;
	}

	return tileTextureValue;
}