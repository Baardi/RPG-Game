#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "TileLayer.h"

void TileLayer::process()
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
			AnimationTile &animationTile = animationTilemap[x][y];

			// Update animation
			ProcessAnimation(sprite, animationTile, clock);
		}
	}

}

void TileLayer::draw(sf::RenderWindow& window)
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
			window.draw(sprite);
		}
	}
}

void TileLayer::loadTexture()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			int tileid = tilemap[x][y];
			if (tileid == 0) // Skip empty tiles
				continue;

			// Specific animated tiles code
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

					animationTileInfo.animationTileData.emplace_back(tile.second, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
				}

				continue;
			}

			int tileTextureValue = GetTextureIndex(tileid);
			if (tileTextureValue == 0) // No texture found
				continue;

			sf::Texture *spriteTexture = tileSets[tileTextureValue];
			{
				int tilex, tiley;
				getTileCoords(spriteTexture, tileid - tileTextureValue, tilex, tiley);
				sf::Sprite sprite(*spriteTexture, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
				sprite.setPosition(x*tileSize.x, y*tileSize.y);
				textureMap[x][y] = sprite;
			}
		}
	}
}

