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
