#include "stdafx.h"
#include "Layer.h"

void Layer::getTileCoords(sf::Texture *texture, int tile, int& x, int& y) const
{
	g_getTileCoords(texture, tile, x, y, tileSize);
}

int Layer::GetTextureIndex(int tileValue) const
{
	int tileTextureValue = 0;

	for (auto &&[tmpKeyVal, texture] : tileSets)
	{
		if (tmpKeyVal > tileTextureValue && tmpKeyVal < tileValue)
			tileTextureValue = tmpKeyVal;
	}

	return tileTextureValue;
}

void Layer::ProcessAnimation(sf::Sprite& sprite, AnimationTile& animationTile, sftools::Chronometer& clock)
{
	auto &animationTileData = animationTile.animationTileData;
	int &currentFrame = animationTile.currentFrame;
	sf::Int32 &lastTime = animationTile.lastTime;

	double currentDuration = clock.getElapsedTime().asMilliseconds();

	if (currentDuration > lastTime + animationTileData[currentFrame].duration)
	{
		do
		{
			lastTime += animationTileData[currentFrame].duration; // To keep it in sync, just increment by the duration
			currentFrame = (currentFrame + 1) % animationTileData.size();
		}
		while (currentDuration > lastTime + animationTileData[currentFrame].duration);

		sf::IntRect &rect = animationTileData[currentFrame].intRect;
		sprite.setTextureRect(rect);
	}
}

void g_getTileCoords(sf::Texture* texture, int tile, int& x, int& y, const TileSize tileSize)
{
	int tileXcount = texture->getSize().x / (tileSize.x + tileSize.s);

	x = (tile % tileXcount) * (tileSize.x + tileSize.s);
	y = (tile / tileXcount) * (tileSize.x + tileSize.s);
}
