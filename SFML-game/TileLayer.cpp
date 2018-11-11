#include "stdafx.h"
#include "TileLayer.h"

TileLayer::TileLayer(const TileSize& tileSize, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles, sftools::Chronometer &clock):
	Layer(tileSize, tileSets, animatedTiles), clock(clock)
{
}

TileLayer::~TileLayer()
{
}

void TileLayer::load(const Json::Value& layer)
{
	width = layer["width"].asInt();
	height = layer["height"].asInt();
	name = layer["name"].asString();
	visible = layer["visible"].asBool();
	opacity = layer["opacity"].asFloat();

	// Prepare tilemap
	auto &data = layer["data"];
	initArrays(data.size());

	// Read in tilemap
	for (unsigned int i = 0; i < data.size(); i++)
	{
		tilemap[i] = data[i].asInt();
	}

	loadTexture();
	LoadProperties(layer);
}

void TileLayer::process()
{
	// Render each tile
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int tileid = getValue(tilemap, x, y);

			// Skip empty tiles
			if (tileid == 0)
				continue;

			sf::Sprite &sprite = getValue(textureMap, x, y);
			AnimationTile &animationTile = getValue(animationTilemap, x, y);

			// Update animation
			if (!animationTile.animationTileData.empty())
				ProcessAnimation(sprite, animationTile, clock);
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
			int tileid = getValue(tilemap, x, y);

			// Skip empty tiles
			if (tileid == 0)
				continue;

			sf::Sprite &sprite = getValue(textureMap, x, y);
			window.draw(sprite);
		}
	}
}

void TileLayer::loadTexture()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int tileid = getValue(tilemap, x, y);
			if (!tileid) 
				continue;		// Skip empty tiles

			int tileTextureValue = GetTextureIndex(tileid);
			if (!tileTextureValue) 
				continue;		// No texture found

			sf::Texture *spriteTexture = tileSets[tileTextureValue];

			// Check if theres animation
			auto it = animatedTiles.find(tileid);
			if (it == animatedTiles.end())
			{
				LoadSpriteTexture(*spriteTexture, tileid - tileTextureValue, x, y);
			}
			else
			{
				// First animationtile decides first texturerect
				auto &animationTile = it->second;
				LoadSpriteTexture(*spriteTexture, animationTile[0].first, x, y);
				LoadSpriteAnimation(*spriteTexture, animationTile, x, y);
			}
		}
	}
}

void TileLayer::LoadSpriteTexture(sf::Texture &texture, int tileid, int x, int y)
{
	int tilex, tiley;
	getTileCoords(&texture, tileid, tilex, tiley);

	auto &sprite = getValue(textureMap, x, y);
	sprite.setColor(sf::Color(255, 255, 255, (256 * opacity) - 1));
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	sprite.setPosition(x*tileSize.x, y*tileSize.y);
}

void TileLayer::LoadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, int>> &animationTile, int x, int y)
{
	auto &animationTileInfo = getValue(animationTilemap, x, y);
	for (const auto &tile : animationTile)
	{
		int tilex, tiley;
		getTileCoords(&texture, tile.first, tilex, tiley);
		animationTileInfo.animationTileData.emplace_back(tile.second, sf::IntRect(tilex, tiley, tileSize.x, tileSize.y));
	}
}

void TileLayer::initArrays(int size)
{
	tilemap.resize(size);
	animationTilemap.resize(size);
	textureMap.resize(size);
}

template <class T>
T& TileLayer::getValue(std::vector<T> &arr, int x, int y)
{
	return arr[x + y * width];
}

template <class T>
const T& TileLayer::getValue(const std::vector<T> &arr, int x, int y)
{
	return arr[x + y * width];
}

bool TileLayer::containsTextureTileCoords(int x, int y) const
{
	if (x >= width || y >= height || x < 0 || y < 0)
		return true; // Out of bounds

	return bool(tilemap[x + y * width]); // if (value is 0) => false, else => true
}

bool TileLayer::containsTexture(double x, double y) const
{
	// Checks the tile the centre of the player is in
	return containsTextureTileCoords(0.5 + x / (tileSize.x + tileSize.s), 0.5 + y / (tileSize.y + tileSize.s));
}
