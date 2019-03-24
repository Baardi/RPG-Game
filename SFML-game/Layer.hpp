#pragma once
#include "MapProperties.hpp"

using AnimationTileMap = std::map<int, std::vector< std::pair<int, sf::Time>>>;

struct AnimationTileData
{
	AnimationTileData(sf::Time duration, sf::IntRect intRect) : duration(duration), intRect(intRect) {}
	
	sf::Time duration;
	sf::IntRect intRect;

	float	  duration_s()	{ return duration.asSeconds(); }
	sf::Int32 duration_ms() { return duration.asMilliseconds(); }
	sf::Int64 duration_ys() { return duration.asMicroseconds(); }
};

struct AnimationTile
{
	int currentFrame = 0;
	sf::Time lastTime;
	std::vector<AnimationTileData> animationTileData;
};

// Small helper struct that contains tile size information
struct TileSize
{
	TileSize() = default;
	TileSize(int size) : x(size), y(size), s(0) {}
	TileSize(int size, int spacing) : x(size), y(size), s(spacing) {}
	TileSize(int xSize, int ySize, int spacing) : x(xSize), y(ySize), s(spacing) {}
	int x; // Width
	int y; // Height
	int s; // Spacing
};

class Layer : public MapProperties
{
public:
	Layer(const TileSize &tileSize) : tileSize(tileSize) {}

	virtual ~Layer() = default;
	
	virtual void process(const sftools::Chronometer &clock) {}
	virtual void draw(sf::RenderTarget& window) {}
	virtual void loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles) {}

	std::string name;
	bool visible;
	std::string type;
	float opacity;

protected:
	// Calculate x and y position of given tile in the texture
	sf::Vector2i getTileCoords(sf::Texture &texture, int tile) const;
	static int GetTextureIndex(int tileValue, const std::map<int, sf::Texture*>& tileSets);
	static void processAnimation(sf::Sprite &sprite, AnimationTile &animationTile, const sftools::Chronometer& clock);
	
	TileSize tileSize;
};

sf::Vector2i g_getTileCoords(sf::Texture &texture, int tile, const TileSize tileSize);
