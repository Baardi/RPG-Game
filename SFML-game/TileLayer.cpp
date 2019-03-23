#include "stdafx.h"
#include "TileLayer.hpp"

TileLayer::TileLayer(const TileSize& tileSize) : Layer(tileSize) {}
TileLayer::~TileLayer() {}

void TileLayer::load(const Json::Value& layer, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
{
	m_width = layer["width"].asInt();
	m_height = layer["height"].asInt();
	name = layer["name"].asString();
	visible = layer["visible"].asBool();
	opacity = layer["opacity"].asFloat();

	// Prepare tilemap
	auto &data = layer["data"];
	initArrays(data.size());

	// Read in tilemap
	for (unsigned int i = 0; i < data.size(); i++)
	{
		m_tilemap[i] = data[i].asInt();
	}

	loadTexture(tileSets, animatedTiles);
	loadProperties(layer);
}

void TileLayer::process(const sftools::Chronometer &clock)
{
	// Render each tile
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int tileid = getValue(m_tilemap, x, y);

			// Skip empty tiles
			if (tileid == 0)
				continue;

			sf::Sprite &sprite = getValue(m_textureMap, x, y);
			AnimationTile &animationTile = getValue(m_animationTilemap, x, y);

			// Update animation
			if (!animationTile.animationTileData.empty())
				processAnimation(sprite, animationTile, clock);
		}
	}
}

void TileLayer::draw(sf::RenderTarget& window)
{
	// Render each tile
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int tileid = getValue(m_tilemap, x, y);

			// Skip empty tiles
			if (tileid == 0)
				continue;

			sf::Sprite &sprite = getValue(m_textureMap, x, y);
			window.draw(sprite);
		}
	}
}

void TileLayer::loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int tileid = getValue(m_tilemap, x, y);
			if (!tileid) 
				continue;		// Skip empty tiles

			int tileTextureValue = GetTextureIndex(tileid, tileSets);
			if (!tileTextureValue) 
				continue;		// No texture found

			sf::Texture *spriteTexture = tileSets[tileTextureValue];

			// Check if theres animation
			auto it = animatedTiles.find(tileid);
			if (it == animatedTiles.end())
			{
				loadSpriteTexture(*spriteTexture, tileid - tileTextureValue, x, y);
			}
			else
			{
				// First animationtile decides first texturerect
				auto &animationTile = it->second;
				loadSpriteTexture(*spriteTexture, animationTile[0].first, x, y);
				loadSpriteAnimation(*spriteTexture, animationTile, x, y);
			}
		}
	}
}

void TileLayer::loadSpriteTexture(sf::Texture &texture, int tileid, int x, int y)
{
	auto [tilex, tiley] = getTileCoords(texture, tileid);

	auto &sprite = getValue(m_textureMap, x, y);
	sprite.setColor(sf::Color(255, 255, 255, (256 * opacity) - 1));
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	sprite.setPosition(x*tileSize.x, y*tileSize.y);
}

void TileLayer::loadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, sf::Time>> &animationTile, int x, int y)
{
	auto &animationTileInfo = getValue(m_animationTilemap, x, y);
	for (const auto [tileid, duration]: animationTile)
	{
		auto [tilex, tiley] = getTileCoords(texture, tileid);
		animationTileInfo.animationTileData.emplace_back(duration, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	}
}

void TileLayer::initArrays(int size)
{
	m_tilemap.resize(size);
	m_animationTilemap.resize(size);
	m_textureMap.resize(size);
}

template <class T>
T& TileLayer::getValue(std::vector<T> &arr, int x, int y)
{
	return arr[x + y * m_width];
}

template <class T>
const T& TileLayer::getValue(const std::vector<T> &arr, int x, int y) const
{
	return arr[x + y * m_width];
}

bool TileLayer::containsTextureTileCoords(int x, int y) const
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0)
		return true; // Out of bounds

	return bool(m_tilemap[x + y * m_width]); // if (value is 0) => false, else => true
}

bool TileLayer::containsTexture(double x, double y) const
{
	// Checks the tile the centre of the player is in
	return containsTextureTileCoords(0.5 + x / (tileSize.x + tileSize.s), 0.5 + y / (tileSize.y + tileSize.s));
}
