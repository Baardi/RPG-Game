#include "stdafx.h"
#include "ObjectSprite.hpp"
#include "State.hpp"
#include "sfUtility.hpp"
#include <corecrt_math_defines.h>
#include "map.hpp"

void ObjectSprite::load(const Json::Value& layer, const Json::Value& object, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
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
	verflip = json_gid / (Map::FLIP_MULTIPLIER * 2);
	horflip = json_gid % (Map::FLIP_MULTIPLIER * 2) > Map::FLIP_MULTIPLIER;
	gid = json_gid % Map::FLIP_MULTIPLIER;

	x = object["x"].asFloat();
	y = object["y"].asFloat();

	if (gid)
	{
		x += height * std::sin(rotation * (M_PI / 180.0));
		y -= height * std::cos(rotation * (M_PI / 180.0));
	}

	m_localBounds = sf::FloatRect(0, 0, width, height);

	m_transform.rotate(rotation);
	m_transform.translate(x, y);

	auto textValue = object["text"];
	if (!textValue.empty() && !gid)
		loadText(textValue);

	loadProperties(object);
	loadTexture(tileSets, animatedTiles);
}

void ObjectSprite::loadText(const Json::Value &textValue)
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

void ObjectSprite::process(const sftools::Chronometer &clock)
{
	if (gid == 0)
		return;

	// Update animation
	if (!m_animationTileInfo.animationTileData.empty())
		processAnimation(sprite, m_animationTileInfo, clock);
}

void ObjectSprite::draw(sf::RenderTarget &window)
{
	if (gid) 
		window.draw(sprite);
	else if (text)
		window.draw(*text);
}

void ObjectSprite::loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
{
	if (!gid)
		return;

	int tileTextureValue = GetTextureIndex(gid, tileSets);
	if (!tileTextureValue) // No texture found
		return;

	sf::Texture *spriteTexture = tileSets[tileTextureValue];

	// Check if theres animation
	auto it = animatedTiles.find(gid);
	if (it == animatedTiles.end())
	{
		loadSpriteTexture(*spriteTexture, gid - tileTextureValue);
	}
	else 
	{
		// First animationtile decides first texturerect
		auto &animationTile = it->second;
		loadSpriteTexture(*spriteTexture, animationTile[0].first);
		loadSpriteAnimation(*spriteTexture, animationTile);
	}
}

sf::FloatRect ObjectSprite::getLocalBounds() const
{
	return m_localBounds;
}

sf::Transform ObjectSprite::getTransform() const
{
	return m_transform;
}

void ObjectSprite::loadSpriteTexture(sf::Texture &texture, int tileid)
{
	auto [tilex, tiley] = getTileCoords(texture, tileid);
	auto textureRect = getTextureRectToUse(tilex, tiley, verflip, horflip);

	sprite.setColor(sf::Color(255, 255, 255, (256 * opacity) - 1));
	sprite.setTexture(texture);
	sprite.setTextureRect(textureRect);
	sprite.setPosition(x, y);
	sprite.setRotation(rotation);
	sprite.setScale(width / static_cast<float>(tileSize.x), height / static_cast<float>(tileSize.y));
}

void ObjectSprite::loadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, sf::Time>> &animationTile)
{
	for (const auto &tile : animationTile)
	{
		auto [tilex, tiley] = getTileCoords(texture, tile.first);
		auto textureRect = getTextureRectToUse(tilex, tiley, verflip, horflip);

		m_animationTileInfo.animationTileData.emplace_back(tile.second, textureRect);
	}
}

sf::IntRect ObjectSprite::getTextureRectToUse(int tilex, int tiley, bool verflip, bool horflip) const // Set texture rect differently depending on flip
{
	int txXPos = verflip ? tilex + tileSize.x : tilex;
	int txYPos = horflip ? tiley + tileSize.y : tiley;
	int txXSize = verflip ? -tileSize.x : tileSize.x;
	int txYSize = horflip ? -tileSize.y : tileSize.y;

	return sf::IntRect(txXPos, txYPos, txXSize, txYSize);
}
