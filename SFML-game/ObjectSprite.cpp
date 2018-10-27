#include "stdafx.h"
#include "ObjectSprite.h"
#include "State.h"
#include "sfUtility.h"
#include <corecrt_math_defines.h>
#include "map.h"

void ObjectSprite::load(Json::Value& layer, Json::Value& object)
{
	// Load basic object info
	name = object["name"].asString();

	width = object["width"].asFloat();
	height = object["height"].asFloat();
	rotation = object["rotation"].asFloat();
	type = object["type"].asString();
	visible = object["visible"].asBool();
	opacity = layer["opacity"].asFloat();

	unsigned int json_gid = object["gid"].asUInt();
	verflip = json_gid / (Map::flipMultiplier * 2);
	horflip = json_gid % (Map::flipMultiplier * 2) > Map::flipMultiplier;
	gid = json_gid % Map::flipMultiplier;

	x = object["x"].asFloat();
	y = object["y"].asFloat();

	if (gid)
	{
		x += height * sin(rotation * (M_PI / 180.0));
		y -= height * cos(rotation * (M_PI / 180.0));
	}

	globalBounds = sf::DoubleRect(x, y, width, height);

	auto textValue = object["text"];
	if (!textValue.empty() && !gid)
		loadText(textValue);

	LoadProperties(object);
	loadTexture();
}

void ObjectSprite::loadText(Json::Value &textValue)
{
	text.emplace();

	text->setFillColor(sf::utility::parseColor(textValue["color"].asString()));
	text->setString(textValue["text"].asString());
	text->setCharacterSize(textValue["pixelsize"].asInt());
	text->setFont(State::Font());
	text->setPosition(x, y);
	text->setRotation(rotation);
	text->setStyle(sf::utility::parseTextStyle(textValue));
}

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
	auto it = animatedTiles.find(gid);
	if (it == animatedTiles.end())
	{
		LoadSpriteTexture(*spriteTexture, gid - tileTextureValue);
	}
	else 
	{
		// First animationtile decides first texturerect
		auto &animationTile = it->second;
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
	sprite.setScale(width / static_cast<float>(tileSize.x), height / static_cast<float>(tileSize.y));
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
