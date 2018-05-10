#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "layer.h"


void Layer::draw(sf::RenderWindow& window)
{
	// Render each tile
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int tileid = tilemap[x][y];

			// Skip empty tiles
			if (tileid == 0)
				continue;

			sf::Sprite &sprite = textureMap[x][y];

			// Update animation
			{
				auto &animationTile = animationTilemap[x][y];
				auto &animationTileData = animationTile.animationTileData;
				if (animationTileData.size())
				{
					int &currentFrame = animationTile.currentFrame;
					double &lastTime = animationTile.lastTime;

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
	}
}

void Layer::loadTexture()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int tileid = tilemap[x][y];
			if (tileid == 0) // Skip empty tiles
				continue;

			// Specific animated tiles code
			{
				auto animationTile = animatedTiles[tileid];
				if (animationTile.size() != 0)
				{
					int tileTextureValue = GetTextureIndex(tileid);

					sf::Texture *spriteTexture = tileSets[tileTextureValue];  // first animationtile decides texture
					{
						int tilex, tiley;
						getTileCoords(spriteTexture, animationTile[0].first, tilex, tiley);
						sf::Sprite sprite(*spriteTexture, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
						sprite.setPosition(x*tileSize.x, y*tileSize.y);
						textureMap[x][y] = sprite;
					}

					auto &animationTileInfo = animationTilemap[x][y];
					for (const auto &tile : animationTile)
					{
						int tilex, tiley;
						getTileCoords(spriteTexture, tile.first, tilex, tiley);

						AnimationTileData animationTileData;
						animationTileData.duration = tile.second;
						animationTileData.intRect = sf::IntRect(tilex, tiley, tileSize.x, tileSize.y);

						animationTileInfo.animationTileData.push_back(animationTileData);
					}

					continue;
				}
			}

			int tileTextureValue = GetTextureIndex(tileid);
			if (tileTextureValue == 0) // No texture found
				continue;

			sf::Texture *spriteTexture = tileSets[tileTextureValue];

			// Texture coordinates
			int tilex, tiley;
			getTileCoords(spriteTexture, tileid - tileTextureValue, tilex, tiley);

			// Using vertex arrays or permanent sprites would be faster
			sf::Sprite sprite(*spriteTexture, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
			sprite.setPosition(x*tileSize.x, y*tileSize.y);

			textureMap[x][y] = sprite;
		}
	}
}

