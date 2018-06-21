#include "stdafx.h"
#include "ObjectSprite.h"
#include <SFML/Graphics/RenderWindow.hpp>

void ObjectSprite::process()
{
	if (gid == 0)
		return;

	// Update animation
	if (!animationTileInfo.animationTileData.empty())
		ProcessAnimation(sprite, animationTileInfo, clock);
}

void ObjectSprite::draw(sf::RenderWindow& window)
{
	if (gid == 0)
		return;

	window.draw(sprite);
}

void ObjectSprite::loadTexture()
{
	globalBounds = sf::FloatRect(x, y, width, height);

	// try loading animation first
	auto animationTile = animatedTiles[gid];
	if (animationTile.size() != 0)
	{
		int tileTextureValue = GetTextureIndex(gid);

		sf::Texture *spriteTexture = tileSets[tileTextureValue];  // first animationtile decides texture
		{
			int tilex, tiley;
			getTileCoords(spriteTexture, animationTile[0].first, tilex, tiley);

			sprite = sf::Sprite(*spriteTexture, sf::IntRect(verflip ? tilex + tileSize.x : tilex, horflip ? tiley + tileSize.y : tiley, verflip ? -tileSize.x : tileSize.x, horflip ? -tileSize.y : tileSize.y)); // flips easy to implement, unreadable code due to stupid way to solve for tiled program
			sprite.setPosition(x, y);
			sprite.setRotation(rotation);
			sprite.setScale(width / float(tileSize.x), height / float(tileSize.y));
		}

		for (const auto &tile : animationTile)
		{
			int tilex, tiley;
			getTileCoords(spriteTexture, tile.first, tilex, tiley);

			animationTileInfo.animationTileData.emplace_back(tile.second, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
		}
		return;
	}

	int tileTextureValue = GetTextureIndex(gid);
	if (!tileTextureValue) // No texture found
		return;

	sf::Texture *texture = tileSets[tileTextureValue];
	{
		int tilex, tiley;
		getTileCoords(texture, gid - tileTextureValue, tilex, tiley);

		sprite = sf::Sprite(*texture, sf::IntRect(verflip ? tilex + tileSize.x : tilex, horflip ? tiley + tileSize.y : tiley, verflip ? -tileSize.x : tileSize.x, horflip ? -tileSize.y : tileSize.y)); // flips easy to implement, unreadable code due to stupid way to solve for tiled program
		sprite.setPosition(x, y);
		sprite.setRotation(rotation);
		sprite.setScale(width / float(tileSize.x), height / float(tileSize.y));
	}
}

sf::FloatRect ObjectSprite::GetGlobalBounds()
{
	return globalBounds;
}

std::string ObjectSprite::GetPropertyValue(const std::string &propertyName)
{
	return propertyMap[propertyName];
}
