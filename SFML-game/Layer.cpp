#include "stdafx.h"
#include "Layer.hpp"
#include "TileSet.hpp"

sf::Vector2i Layer::getTileCoords(const sf::Texture &texture, int tile) const
{
	return g_getTileCoords(texture, tile, tileSize);
}

int Layer::getTextureIndex(int tileValue, const std::map<int, TileSet>& tileSets)
{
	int tileTextureValue = 0;

	for (auto [tmpKeyVal, texture] : tileSets)
	{
		if (tmpKeyVal > tileTextureValue && tmpKeyVal < tileValue)
			tileTextureValue = tmpKeyVal;
	}

	return tileTextureValue;
}

void Layer::processAnimation(sf::Sprite& sprite, AnimationTile& animationTile, const sftools::Chronometer& clock)
{
	auto &animationTileData = animationTile.data;
	int &currentFrame = animationTile.currentFrame;
	sf::Time &lastTime = animationTile.lastTime;

	sf::Time currentTime = clock.getElapsedTime();

	if (currentTime > lastTime + animationTileData[currentFrame].duration)
	{
		do
		{
			lastTime += animationTileData[currentFrame].duration; // To keep it in sync, just increment by the duration
			currentFrame = (currentFrame + 1) % animationTileData.size();
		}
		while (currentTime > lastTime + animationTileData[currentFrame].duration);

		sf::IntRect &rect = animationTileData[currentFrame].intRect;
		sprite.setTextureRect(rect);
	}
}

sf::Vector2i g_getTileCoords(const sf::Texture &texture, int tile, const TileSize tileSize)
{
	int tileXcount = texture.getSize().x / (tileSize.x + tileSize.s);

	int x = (tile % tileXcount) * (tileSize.x + tileSize.s);
	int y = (tile / tileXcount) * (tileSize.x + tileSize.s);

	return sf::Vector2i(x, y);
}
