#include "stdafx.h"
#include "TileLayer.hpp"
#include "TileSet.hpp"
#include "FringeDrawer.hpp"

void TileLayer::load(const Json::Value &layer, const std::map<int, TileSet> &tileSets)
{
	type = layer["type"].asString();
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

	loadTexture(tileSets);
	loadProperties(layer["properties"]);
}

void TileLayer::save(Json::Value &layers) const
{
	Json::Value layer;

	layer["type"] = "tilelayer";
	layer["name"] = name;
	layer["id"] = id;
	layer["x"] = 0;
	layer["y"] = 0;
	layer["width"] = width;
	layer["height"] = height;
	layer["opacity"] = opacity;
	layer["visible"] = visible;

	for (unsigned int i = 0; i < m_tilemap.size(); i++)
	{
		layer["data"].append(m_tilemap[i].id);
	}

	saveProperties(layer["properties"]);

	layers.append(layer);
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

void TileLayer::draw(sf::RenderTarget &target)
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

			target.draw(tile.sprite);
		}
	}
}

void TileLayer::drawWithFringe(sf::RenderTarget &target, FringeDrawer &fringeDrawer)
{	
	// Render each tile
	for (int y = 0; y < height; y++)
	{
		fringeDrawer.drawBetween(target, y*tileSize.y, (y+1)*tileSize.y);
		for (int x = 0; x < width; x++)
		{
			auto &tile = m_tilemap[x + y * width];
			
			// Skip empty tiles
			if (tile.id == 0)
				continue;

			target.draw(tile.sprite);
		}
	}
}

void TileLayer::loadTexture(const std::map<int, TileSet> &tileSets)
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

			tile.tileset = &tileSets.find(tileTextureValue)->second;
			const sf::Texture &spriteTexture = *tile.tileset->texture;
			const AnimationTileMap &animatedTiles = tile.tileset->animatedTiles;

			// Check if theres animation
			auto it = animatedTiles.find(tile.id);
			if (it == animatedTiles.end() || it->second.empty())
			{
				loadSpriteTexture(tile, spriteTexture, tile.id - tileTextureValue, x, y);
			}
			else
			{
				// First animationtile decides first texturerect
				auto &animationTile = it->second;
				loadSpriteTexture(tile, spriteTexture, animationTile[0].first, x, y);
				loadSpriteAnimation(spriteTexture, tile, animationTile);
			}
		}
	}
}

void TileLayer::loadSpriteTexture(Tile &tile, const sf::Texture &texture, int tileid, int x, int y)
{
	auto [tilex, tiley] = getTileCoords(texture, tileid, tile.tileset->tileSize);
	auto tileset = tile.tileset;

	auto &sprite = tile.sprite;
	sprite.setColor(sf::Color(255, 255, 255, (256 * opacity) - 1));
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tilex, tiley, tileset->tileSize.x, tileset->tileSize.y));
	sprite.setPosition(x*tileSize.x + tileSize.x-tileset->tileSize.x, y*tileSize.y + tileSize.y - tileset->tileSize.y);
}

void TileLayer::loadSpriteAnimation(const sf::Texture &texture, Tile &tile, const std::vector<std::pair<int, sf::Time>> &animationTile)
{
	for (const auto [tileid, duration]: animationTile)
	{
		auto [tilex, tiley] = getTileCoords(texture, tileid, tile.tileset->tileSize);
		auto rect = sf::IntRect(tilex, tiley, tile.tileset->tileSize.x, tile.tileset->tileSize.y);
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
