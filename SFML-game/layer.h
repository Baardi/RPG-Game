#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

namespace sf
{
	class RenderWindow;
	class Texture;
}

using AnimationTileMap = std::unordered_map<int, std::vector< std::pair<int, int>> >;

struct AnimationTileData
{
	AnimationTileData(int duration, sf::IntRect intRect) : duration(duration), intRect(intRect) {}
	int duration;
	sf::IntRect intRect;
};

struct AnimationTile
{
	int currentFrame = 0;
	sf::Int32 lastTime = 0;
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

class Layer
{
	friend class Map;
public:
	Layer(const TileSize &tileSize, std::unordered_map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles) :
		tileSize(tileSize), tileSets(tileSets), animatedTiles(animatedTiles) {}

	virtual ~Layer() = default;
	
	virtual void process() {}
	virtual void draw(sf::RenderWindow& window) {}
	virtual void loadTexture() {}

protected:

	// Calculate x and y position of given tile in the texture
	void getTileCoords(sf::Texture *texture, int tile, int& x, int& y) const;
	int GetTextureIndex(int tileValue) const;
	static void ProcessAnimation(sf::Sprite &sprite, AnimationTile &animationTile, sf::Clock &clock);
	
	const TileSize &tileSize;
	std::unordered_map<int, sf::Texture *> &tileSets;
	AnimationTileMap &animatedTiles;

	std::string name;
	std::string type;
	bool visible;
	float opacity;
};
