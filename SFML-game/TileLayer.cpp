#include "stdafx.h"
#include "TileLayer.hpp"

TileLayer::TileLayer(const TileSize& tileSize) : Layer(tileSize) {}
TileLayer::~TileLayer() {}

void TileLayer::load(const Json::Value& layer, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
{
	width = layer["width"].asInt();
	height = layer["height"].asInt();
	name = layer["name"].asString();
	visible = layer["visible"].asBool();
	opacity = layer["opacity"].asFloat();
	id = layer["id"].asInt();

	// Prepare tilemap
	auto &data = layer["data"];
	m_tilemap.resize(data.size());

	// Read in tilemap
	for (unsigned int i = 0; i < data.size(); i++)
	{
		m_tilemap[i].id = data[i].asInt();
	}

	loadTexture(tileSets, animatedTiles);
	loadProperties(layer["properties"]);
}

void TileLayer::save(Json::Value &value) const
{
}

void TileLayer::process(const sftools::Chronometer &clock)
{
	// Render each tile
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			auto &tile = m_tilemap[x + y * width];

			// Skip empty tiles
			if (tile.id == 0)
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
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			auto &tile = m_tilemap[x + y * width];
			
			// Skip empty tiles
			if (tile.id == 0)
				continue;

			window.draw(tile.sprite);
		}
	}
}

void TileLayer::loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			auto &tile = m_tilemap[x + y * width];
			
			if (!tile.id) 
				continue;		// Skip empty tiles

			int tileTextureValue = getTextureIndex(tile.id, tileSets);
			if (!tileTextureValue) 
				continue;		// No texture found

			sf::Texture *spriteTexture = tileSets[tileTextureValue];

			// Check if theres animation
			auto it = animatedTiles.find(tile.id);
			if (it == animatedTiles.end())
			{
				loadSpriteTexture(tile.sprite, *spriteTexture, tile.id - tileTextureValue, x, y);
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
	if (x >= width || y >= height || x < 0 || y < 0)
		return true; // Out of bounds

	const auto &tile = m_tilemap[x + y * width];
	return bool(tile.id); // if (value is 0) => false, else => true
}

bool TileLayer::containsTexture(double x, double y) const
{
	// Checks the tile the centre of the player is in
	return containsTextureTileCoords(0.5 + x / (tileSize.x + tileSize.s), 0.5 + y / (tileSize.y + tileSize.s));
}
