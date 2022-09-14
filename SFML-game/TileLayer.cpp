#include "stdafx.h"
#include "TileLayer.hpp"
#include "TileSet.hpp"
#include "FringeDrawer.hpp"

bool TileLayer::load(const Json::Value &layer, const std::map<int, TileSet> &tileSets)
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

	return true;
}

bool TileLayer::save(Json::Value &layers) const
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

	return true;
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

	fringeDrawer.drawBetween(target, height*tileSize.y, (height+1)*tileSize.y);
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
				loadSpriteTexture(tile, spriteTexture, tile.id - tileTextureValue, { x, y });
			}
			else
			{
				// First animationtile decides first texturerect
				auto &animationTile = it->second;
				loadSpriteTexture(tile, spriteTexture, animationTile[0].tileid, { x, y });
				loadSpriteAnimation(spriteTexture, tile, animationTile);
			}
		}
	}
}

void TileLayer::loadSpriteTexture(Tile &tile, const sf::Texture &texture, int tileid, sf::Vector2i pos)
{
	const auto coord = getTileCoords(texture, tileid, tile.tileset->tileSize);
	const auto *tileset = tile.tileset;

	auto &sprite = tile.sprite;
	sprite.setColor(sf::Color(255U, 255U, 255U, static_cast<std::uint8_t>(255U * opacity) - 1U));
	sprite.setTexture(texture);
	sprite.setTextureRect({ coord, { tileset->tileSize.x, tileset->tileSize.y } });
	sprite.setPosition({ static_cast<float>(pos.x * tileSize.x + tileSize.x - tileset->tileSize.x), static_cast<float>(pos.y * tileSize.y + tileSize.y - tileset->tileSize.y) });
}

void TileLayer::loadSpriteAnimation(const sf::Texture &texture, Tile &tile, const Animation &animation)
{
	for (const auto &[tileid, duration] : animation)
	{
		const auto coord = getTileCoords(texture, tileid, tile.tileset->tileSize);
		const auto rect = sf::IntRect{ coord, { tile.tileset->tileSize.x, tile.tileset->tileSize.y }};
		tile.animation.data.emplace_back(duration, rect);
	}
}

bool TileLayer::containsTextureTileCoords(sf::Vector2i pos) const
{
	if (pos.x >= width || pos.y >= height || pos.x < 0 || pos.y < 0)
		return true; // Out of bounds

	const auto &tile = m_tilemap[pos.x + pos.y * width];
	return static_cast<bool>(tile.id); // if (value is 0) => false, else => true
}

bool TileLayer::containsTexture(sf::Vector2f pos) const
{
	if ((pos.x+1) > (width-1) * tileSize.x || (pos.y+1) > (height-1) * tileSize.y || pos.x < 0 || pos.y < 0)
		return true; // Out of bounds

	// Checks the tile the centre of the player is in
	return containsTextureTileCoords({ static_cast<int>(std::floorf(0.5f + pos.x / static_cast<float>(tileSize.x + tileSize.s))), static_cast<int>(std::floorf(0.5f + pos.y / static_cast<float>(tileSize.y + tileSize.s))) });
}
