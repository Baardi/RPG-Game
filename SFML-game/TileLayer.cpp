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
	m_tilemap.resize(data.size());

	// Read in tilemap
	for (unsigned int i = 0; i < data.size(); i++)
	{
		m_tilemap[i].gid = data[i].asInt();
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
			auto &tile = m_tilemap[x + y * m_width];

			// Skip empty tiles
			if (tile.gid == 0)
				continue;
			
			// Update animation
			if (!tile.animation.data.empty())
				processAnimation(tile.sprite, tile.animation, clock);
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
			auto &tile = m_tilemap[x + y * m_width];
			
			// Skip empty tiles
			if (tile.gid == 0)
				continue;

			window.draw(tile.sprite);
		}
	}
}

void TileLayer::loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			auto &tile = m_tilemap[x + y * m_width];
			
			if (!tile.gid) 
				continue;		// Skip empty tiles

			int tileTextureValue = GetTextureIndex(tile.gid, tileSets);
			if (!tileTextureValue) 
				continue;		// No texture found

			sf::Texture *spriteTexture = tileSets[tileTextureValue];

			// Check if theres animation
			auto it = animatedTiles.find(tile.gid);
			if (it == animatedTiles.end())
			{
				loadSpriteTexture(tile.sprite, *spriteTexture, tile.gid - tileTextureValue, x, y);
			}
			else
			{
				// First animationtile decides first texturerect
				auto &animationTile = it->second;
				loadSpriteTexture(tile.sprite, *spriteTexture, animationTile[0].first, x, y);
				loadSpriteAnimation(*spriteTexture, tile, animationTile);
			}
		}
	}
}

void TileLayer::loadSpriteTexture(sf::Sprite &sprite, const sf::Texture &texture, int tileid, int x, int y)
{
	auto [tilex, tiley] = getTileCoords(texture, tileid);

	sprite.setColor(sf::Color(255, 255, 255, (256 * opacity) - 1));
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	sprite.setPosition(x*tileSize.x, y*tileSize.y);
}

void TileLayer::loadSpriteAnimation(const sf::Texture &texture, Tile &tile, std::vector<std::pair<int, sf::Time>> &animationTile)
{
	for (const auto [tileid, duration]: animationTile)
	{
		auto [tilex, tiley] = getTileCoords(texture, tileid);
		auto rect = sf::IntRect(tilex, tiley, tileSize.x, tileSize.y);
		tile.animation.data.emplace_back(duration, rect);
	}
}

bool TileLayer::containsTextureTileCoords(int x, int y) const
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0)
		return true; // Out of bounds

	const auto &tile = m_tilemap[x + y * m_width];
	return bool(tile.gid); // if (value is 0) => false, else => true
}

bool TileLayer::containsTexture(double x, double y) const
{
	// Checks the tile the centre of the player is in
	return containsTextureTileCoords(0.5 + x / (tileSize.x + tileSize.s), 0.5 + y / (tileSize.y + tileSize.s));
}
