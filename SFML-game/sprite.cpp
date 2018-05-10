#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "sprite.h"

void Sprite::process(float deltaTime)
{
	// todo: split draw, and put state changing stuff here. only keep the draw itself in draw
}

void Sprite::draw(sf::RenderWindow& window)
{
	if (id == 0)
		return;

	// Update animation
	{
		auto &animationTileData = animationTileInfo.animationTileData;
		if (animationTileData.size())
		{
			int &currentFrame = animationTileInfo.currentFrame;
			double &lastTime = animationTileInfo.lastTime;

			double currentDuration = clock.getElapsedTime().asMilliseconds();

			if (int(currentDuration - lastTime) > animationTileData[currentFrame].duration)
			{
				lastTime = currentDuration;
				currentFrame = (currentFrame + 1) % animationTileData.size();

				sf::IntRect &rect = animationTileData[currentFrame].intRect;
				sprite.setTextureRect(rect);
			}
		}
	}

	window.draw(sprite);
}

void Sprite::loadTexture()
{
	// try loading animation first
	auto animationTile = animatedTiles[id];
	if (animationTile.size() != 0)
	{
		int tileTextureValue = GetTextureIndex(id);

		sf::Texture *spriteTexture = tileSets[tileTextureValue];  // first animationtile decides texture
		{
			int tilex, tiley;
			getTileCoords(spriteTexture, animationTile[0].first, tilex, tiley);
			sprite = sf::Sprite(*spriteTexture, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
			sprite.setPosition(x*tileSize.x, y*tileSize.y);
		}

		for (const auto &tile : animationTile)
		{
			int tilex, tiley;
			getTileCoords(spriteTexture, tile.first, tilex, tiley);

			AnimationTileData animationTileData;
			animationTileData.duration = tile.second;
			animationTileData.intRect = sf::IntRect(tilex, tiley, tileSize.x, tileSize.y);

			animationTileInfo.animationTileData.push_back(animationTileData);
		}
		return;
	}

	int tileTextureValue = GetTextureIndex(id);
	if (!tileTextureValue) // No texture found
		return;

	sf::Texture *texture = tileSets[tileTextureValue];
	
	int tilex, tiley;
	getTileCoords(texture, id - tileTextureValue, tilex, tiley);

	// Using vertex arrays or permanent sprites would be faster
	sprite = sf::Sprite(*texture, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	sprite.setPosition(float(x), float(y));
}
