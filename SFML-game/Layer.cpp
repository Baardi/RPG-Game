#include "stdafx.h"
#include "Layer.hpp"
#include "TileSet.hpp"


int getTextureIndex(int tileValue, const std::map<int, TileSet>& tileSets)
{
	int tileTextureValue = 0;

	for (auto [tmpKeyVal, texture] : tileSets)
	{
		if (tmpKeyVal > tileTextureValue && tmpKeyVal <= tileValue)
			tileTextureValue = tmpKeyVal;
	}

	return tileTextureValue;
}

void processAnimation(sf::Sprite& sprite, AnimationTile& animationTile, const sftools::Chronometer& clock)
{
	auto &animationTileData = animationTile.data;
	int &currentFrame = animationTile.currentFrame;
	sf::Time &lastTime = animationTile.lastTime;

	sf::Time currentTime = clock.getElapsedTime();

	bool updated = false;
	while (currentTime > lastTime + animationTileData[currentFrame].duration)
	{
		lastTime += animationTileData[currentFrame].duration; // To keep it in sync, just increment by the duration
		currentFrame = (currentFrame + 1) % animationTileData.size();
		
		updated = true;
	}

	if (updated)
	{
		sf::IntRect &rect = animationTileData[currentFrame].intRect;
		sprite.setTextureRect(rect);
	}
}

sf::Vector2i getTileCoords(const sf::Texture &texture, int tile, TileSize tileSize)
{
	int tileXcount = texture.getSize().x / (tileSize.x + tileSize.s);

	int x = tileSize.m + (tile % tileXcount) * (tileSize.x + tileSize.s);
	int y = tileSize.m + (tile / tileXcount) * (tileSize.y + tileSize.s);

	return sf::Vector2i(x, y);
}
