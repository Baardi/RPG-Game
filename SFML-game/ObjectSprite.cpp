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
	if (gid == 0)
		return;

	globalBounds = sf::FloatRect(x, y, width, height);

	// try loading animation first
	auto animationTile = animatedTiles[gid];
	if (animationTile.size() != 0)
	{
		int tileTextureValue = GetTextureIndex(gid);

		sf::Texture &spriteTexture = *tileSets[tileTextureValue];  // first animationtile decides first texture
		LoadSpriteTexture(spriteTexture, animationTile[0].first);
		LoadSpriteAnimation(spriteTexture, animationTile);

		return;
	}

	int tileTextureValue = GetTextureIndex(gid);
	if (!tileTextureValue) // No texture found
		return;

	sf::Texture &spriteTexture = *tileSets[tileTextureValue];
	LoadSpriteTexture(spriteTexture, gid - tileTextureValue);
}

sf::FloatRect ObjectSprite::GetGlobalBounds()
{
	return globalBounds;
}

std::string ObjectSprite::GetPropertyValue(const std::string &propertyName)
{
	return propertyMap[propertyName];
}

void ObjectSprite::LoadSpriteTexture(sf::Texture &texture, int tileid)
{
	int tilex, tiley;
	getTileCoords(&texture, tileid, tilex, tiley);

	sprite.setColor(sf::Color(255, 255, 255, (256 * opacity) - 1));
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(verflip ? tilex + tileSize.x : tilex, horflip ? tiley + tileSize.y : tiley, verflip ? -tileSize.x : tileSize.x, horflip ? -tileSize.y : tileSize.y)); // flips easy to implement, unreadable code due to stupid way to solve for tiled program
	sprite.setPosition(x, y);
	sprite.setRotation(rotation);
	sprite.setScale(width / float(tileSize.x), height / float(tileSize.y));
}

void ObjectSprite::LoadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, int>> &animationTile)
{
	for (const auto &tile : animationTile)
	{
		int tilex, tiley;
		getTileCoords(&texture, tile.first, tilex, tiley);
		animationTileInfo.animationTileData.emplace_back(tile.second, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	}
}
