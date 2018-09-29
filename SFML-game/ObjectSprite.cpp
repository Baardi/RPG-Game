#include "stdafx.h"
#include "ObjectSprite.h"

void ObjectSprite::process()
{
	if (gid == 0)
		return;

	// Update animation
	if (!animationTileInfo.animationTileData.empty())
		ProcessAnimation(sprite, animationTileInfo, clock);
}

void ObjectSprite::draw(sf::RenderWindow &window)
{
	if (gid) 
		window.draw(sprite);
	else if (text)
		window.draw(*text);
}

void ObjectSprite::loadTexture()
{
	if (!gid)
		return;

	int tileTextureValue = GetTextureIndex(gid);
	if (!tileTextureValue) // No texture found
		return;

	sf::Texture *spriteTexture = tileSets[tileTextureValue];

	// Check if theres animation
	auto animationTile = animatedTiles[gid];
	if (animationTile.empty())
	{
		LoadSpriteTexture(*spriteTexture, gid - tileTextureValue);
	}
	else 
	{
		// First animationtile decides first texturerect
		LoadSpriteTexture(*spriteTexture, animationTile[0].first);
		LoadSpriteAnimation(*spriteTexture, animationTile);
	}
}

sf::DoubleRect ObjectSprite::GetGlobalBounds() const
{
	return globalBounds;
}

void ObjectSprite::LoadSpriteTexture(sf::Texture &texture, int tileid)
{
	int tilex, tiley;
	getTileCoords(&texture, tileid, tilex, tiley);
	auto textureRect = GetTextureRectToUse(tilex, tiley, verflip, horflip);

	sprite.setColor(sf::Color(255, 255, 255, (256 * opacity) - 1));
	sprite.setTexture(texture);
	sprite.setTextureRect(textureRect);
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
		auto textureRect = GetTextureRectToUse(tilex, tiley, verflip, horflip);

		animationTileInfo.animationTileData.emplace_back(tile.second, textureRect);
	}
}

sf::IntRect ObjectSprite::GetTextureRectToUse(int tilex, int tiley, bool verflip, bool horflip) const // Set texture rect differently depending on flip
{
	int txXPos = verflip ? tilex + tileSize.x : tilex;
	int txYPos = horflip ? tiley + tileSize.y : tiley;
	int txXSize = verflip ? -tileSize.x : tileSize.x;
	int txYSize = horflip ? -tileSize.y : tileSize.y;

	return sf::IntRect(txXPos, txYPos, txXSize, txYSize);
}
