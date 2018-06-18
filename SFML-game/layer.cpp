#include "stdafx.h"
#include "Layer.h"
#include <SFML/Graphics.hpp>

void Layer::getTileCoords(sf::Texture *texture, int tile, int& x, int& y)
{
	int tileXcount = texture->getSize().x / (tileSize.x + tileSize.s);

	x = (tile % tileXcount) * (tileSize.x + tileSize.s);
	y = (tile / tileXcount) * (tileSize.x + tileSize.s);
}

int Layer::GetTextureIndex(int tileValue)
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

void Layer::ProcessAnimation(sf::Sprite& sprite, AnimationTile& animationTile, sf::Clock& clock)
{
	auto &animationTileData = animationTile.animationTileData;
	if (animationTileData.size())
	{
		int &currentFrame = animationTile.currentFrame;
		double &lastTime = animationTile.lastTime;

		double currentDuration = clock.getElapsedTime().asMilliseconds();

		if (int(currentDuration - lastTime) > animationTileData[currentFrame].duration)
		{
			int inc = 0; // To skip frames if running too slowly, instead of playing catchup
			do
			{
				inc++;
				lastTime += animationTileData[currentFrame].duration; // To keep it in sync, just increment by the duration
			} while (currentDuration > lastTime + animationTileData[currentFrame].duration);

			currentFrame = (currentFrame + inc) % animationTileData.size();

			sf::IntRect &rect = animationTileData[currentFrame].intRect;
			sprite.setTextureRect(rect);
		}
	}
}
