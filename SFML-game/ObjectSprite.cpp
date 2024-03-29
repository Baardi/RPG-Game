#include "stdafx.h"
#include "ObjectSprite.hpp"
#include "sfUtility.hpp"
#include <numbers>
#include "Map.hpp"
#include "App/ResourceManager.hpp"

void ObjectSprite::load(const Json::Value& layer, const Json::Value& object, const std::unordered_map<int, TileSet> &tileSets)
{
	// Load basic object info
	name = object["name"].asString();
	width = object["width"].asFloat();
	height = object["height"].asFloat();
	rotation = object["rotation"].asFloat();
	class_ = object["class"].asString();
	visible = object["visible"].asBool();
	opacity = layer["opacity"].asFloat();
	id = object["id"].asInt();

	unsigned int json_gid = object["gid"].asUInt();
	verflip = json_gid / (Map::FLIP_CONSTANT * 2);
	horflip = json_gid % (Map::FLIP_CONSTANT * 2) > Map::FLIP_CONSTANT;
	gid = json_gid % Map::FLIP_CONSTANT;

	x = object["x"].asFloat();
	y = object["y"].asFloat();

	if (gid)
	{
		x += height * std::sinf(rotation * (std::numbers::pi_v<float> / 180.0f));
		y -= height * std::cosf(rotation * (std::numbers::pi_v<float> / 180.0f));
	}

	m_localBounds = sf::FloatRect({ 0.0f, 0.0f }, { width, height });

	sf::Vector2f scale(1, 1);
	sf::Vector2f origin(0, 0);
	sf::Vector2f translation(x, y);
	m_transform = sf::utility::computeTransform(origin, translation, scale, rotation);

	auto textValue = object["text"];
	if (!textValue.empty())
		loadText(textValue);

	loadProperties(object["properties"]);
	applyProperties();

	loadTexture(tileSets);
}

void ObjectSprite::loadText(const Json::Value &textValue)
{
	text.emplace(resources().font());

	text->setFillColor(sf::utility::parseColor(textValue["color"].asString()));
	text->setString(textValue["text"].asString());
	text->setCharacterSize(textValue["pixelsize"].asInt());
	text->setPosition({ x, y });
	text->setRotation(sf::degrees(rotation));
	text->setStyle(sf::utility::parseTextStyle(textValue));
}

void ObjectSprite::save(Json::Value &objects) const
{
	Json::Value object;

	object["height"] = height;
	object["id"] = id;
	object["name"] = name;
	object["rotation"] = rotation;
	object["class"] = class_;
	object["visible"] = visible;
	object["width"] = width;
	object["height"] = height;

	float json_x = x;
	float json_y = y;
	if (gid)
	{
		json_x -= height * std::sinf(rotation * (std::numbers::pi_v<float> / 180.0f));
		json_y += height * std::cosf(rotation * (std::numbers::pi_v<float> / 180.0f));
	}
	object["x"] = json_x;
	object["y"] = json_y;

	unsigned int json_gid = gid;
	if (horflip) json_gid += Map::FLIP_CONSTANT;
	if (verflip) json_gid += Map::FLIP_CONSTANT * 2;
	object["gid"] = json_gid;

	if (text.has_value())
		saveText(object["text"]);

	saveProperties(object["properties"]);

	objects.append(object);
}

void ObjectSprite::saveText(Json::Value &textValue) const
{
	unsigned int style = text->getStyle();

	if (style & sf::Text::Bold)			textValue["bold"] = true;
	if (style & sf::Text::Italic)		textValue["italic"] = true;
	if (style & sf::Text::StrikeThrough)textValue["strikeout"] = true;
	if (style & sf::Text::Underlined)	textValue["underline"] = true;

	textValue["text"] = std::string(text->getString());
	textValue["pixelsize"] = text->getCharacterSize();
	textValue["color"] = sf::utility::serializeColor(text->getFillColor());
}

void ObjectSprite::process(const sftools::Chronometer &clock)
{
	if (gid == 0)
		return;

	// Update animation
	if (sprite && !m_animationTileInfo.data.empty())
		processAnimation(*sprite, m_animationTileInfo, clock);
}

void ObjectSprite::draw(sf::RenderTarget &target)
{
	if (sprite) 
		target.draw(*sprite);
	else if (text)
		target.draw(*text);

#ifdef _DEBUG
	drawDebugOutline(target);
#endif // _DEBUG
}

void ObjectSprite::loadTexture(const std::unordered_map<int, TileSet> &tileSets)
{
	if (!gid)
		return;

	int tileTextureValue = getTextureIndex(gid, tileSets);
	if (!tileTextureValue) // No texture found
		return;

	tileset = &tileSets.find(tileTextureValue)->second;
	const sf::Texture &spriteTexture = *tileset->texture;
	const AnimationTileMap &animatedTiles = tileset->animatedTiles;

	// Check if theres animation
	auto it = animatedTiles.find(gid);
	if (it == animatedTiles.end())
	{
		loadSpriteTexture(spriteTexture, gid - tileTextureValue);
	}
	else 
	{
		// First animationtile decides first texturerect
		auto &animationTile = it->second;
		loadSpriteTexture(spriteTexture, animationTile[0].tileid);
		loadSpriteAnimation(spriteTexture, animationTile);
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

void ObjectSprite::loadSpriteTexture(const sf::Texture &texture, int tileid)
{
	auto [tilex, tiley] = getTileCoords(texture, tileid, tileset->tileSize);
	auto textureRect = getTextureRectToUse(tileset->tileSize, tilex, tiley, verflip, horflip);

	sprite.emplace(texture);
	sprite->setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(256 * opacity) - 1));
	sprite->setTextureRect(textureRect);
	sprite->setPosition({ x, y });
	sprite->setRotation(sf::degrees(rotation));
	sprite->setScale({ width / static_cast<float>(tileset->tileSize.x), height / static_cast<float>(tileset->tileSize.y) });
}

void ObjectSprite::loadSpriteAnimation(const sf::Texture &texture, const Animation &animation)
{
	for (const auto [tileid, duration] : animation)
	{
		auto [tilex, tiley] = getTileCoords(texture, tileid, tileset->tileSize);
		auto textureRect = getTextureRectToUse(tileset->tileSize, tilex, tiley, verflip, horflip);

		m_animationTileInfo.data.emplace_back(duration, textureRect);
	}
}

sf::IntRect ObjectSprite::getTextureRectToUse(TileSize tileSize, int tilex, int tiley, bool verflip, bool horflip) // Set texture rect differently depending on flip
{
	const int txXPos = verflip ? tilex + tileSize.x : tilex;
	const int txYPos = horflip ? tiley + tileSize.y : tiley;
	const int txXSize = verflip ? -tileSize.x : tileSize.x;
	const int txYSize = horflip ? -tileSize.y : tileSize.y;

	return sf::IntRect({ txXPos, txYPos }, { txXSize, txYSize });
}
