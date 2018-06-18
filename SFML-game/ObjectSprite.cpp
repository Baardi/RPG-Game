#include "stdafx.h"
#include "ObjectSprite.h"
#include <SFML/Graphics/RenderWindow.hpp>

void ObjectSprite::process()
{
	if (id == 0)
		return;

	// Update animation
	ProcessAnimation(sprite, animationTileInfo, clock);
}

void ObjectSprite::draw(sf::RenderWindow& window)
{
	if (id == 0)
		return;

	window.draw(sprite);
}

void ObjectSprite::loadTexture()
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
			sprite.setPosition(float(x), float(y));
			sprite.setRotation(rotation);
		}

		for (const auto &tile : animationTile)
		{
			int tilex, tiley;
			getTileCoords(spriteTexture, tile.first, tilex, tiley);

			animationTileInfo.animationTileData.emplace_back(tile.second, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
		}
		return;
	}

	int tileTextureValue = GetTextureIndex(id);
	if (!tileTextureValue) // No texture found
		return;

	sf::Texture *texture = tileSets[tileTextureValue];

	int tilex, tiley;
	getTileCoords(texture, id - tileTextureValue, tilex, tiley);

	sprite = sf::Sprite(*texture, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y)); // flips easy to implement, unreadable code due to stupid way to solve for tiled program
	sprite.setPosition(float(x), float(y));
	sprite.setRotation(rotation);
}
