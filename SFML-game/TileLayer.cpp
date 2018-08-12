#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "TileLayer.h"

TileLayer::TileLayer(const TileSize& tileSize, std::unordered_map<int, sf::Texture*>& tileSets, AnimationTileMap& animatedTiles, sf::Clock& clock):
	Layer(tileSize, tileSets, animatedTiles), clock(clock)
{
}

TileLayer::~TileLayer()
{
	delete[] tilemap;
	delete[] animationTilemap;
	delete[] textureMap;
}

void TileLayer::process()
{
	// Render each tile
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int tileid = get(tilemap, x, y);

			// Skip empty tiles
			if (tileid == 0)
				continue;

			sf::Sprite &sprite = get(textureMap, x, y);
			AnimationTile &animationTile = get(animationTilemap, x, y);

			// Update animation
			if (!animationTile.animationTileData.empty())
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
			int tileid = get(tilemap, x, y);

			// Skip empty tiles
			if (tileid == 0)
				continue;

			sf::Sprite &sprite = get(textureMap, x, y);
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
			int tileid = get(tilemap, x, y);
			if (tileid == 0) 
				continue;		// Skip empty tiles

			int tileTextureValue = GetTextureIndex(tileid);
			if (tileTextureValue == 0) // No texture found
				continue;

			sf::Texture *spriteTexture = tileSets[tileTextureValue];

			// Specific animated tiles code
			auto animationTile = animatedTiles[tileid];
			if (animationTile.size() != 0)
			{
				LoadSpriteTexture(*spriteTexture, animationTile[0].first, x, y); // first animationtile decides first texture
				LoadSpriteAnimation(*spriteTexture, animationTile, x, y);

				continue;
			}

			LoadSpriteTexture(*spriteTexture, tileid - tileTextureValue, x, y);
		}
	}
}

void TileLayer::LoadSpriteTexture(sf::Texture &texture, int tileid, int x, int y)
{
	int tilex, tiley;
	getTileCoords(&texture, tileid, tilex, tiley);

	auto &sprite = get(textureMap, x, y);
	sprite.setColor(sf::Color(255, 255, 255, (256 * opacity) - 1));
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	sprite.setPosition(x*tileSize.x, y*tileSize.y);
}

void TileLayer::LoadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, int>> &animationTile, int x, int y)
{
	auto &animationTileInfo = get(animationTilemap, x, y);
	for (const auto &tile : animationTile)
	{
		int tilex, tiley;
		getTileCoords(&texture, tile.first, tilex, tiley);
		animationTileInfo.animationTileData.emplace_back(tile.second, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	}
}

void TileLayer::initArrays()
{
	tilemap = new int[width*height];
	animationTilemap = new AnimationTile[width*height];
	textureMap = new sf::Sprite[width*height];
}
