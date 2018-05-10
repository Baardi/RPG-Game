#pragma once
#include <map>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

namespace sf
{
	class RenderWindow;
	class Texture;
}

struct AnimationTileData
{
	sf::IntRect intRect;
	int duration;
};

struct AnimationTile
{
	int currentFrame = 0;
	double lastTime = 0;
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

class MapObject
{
	friend class Map;
public:
	MapObject(TileSize tileSize, std::map<int, sf::Texture *> &tileSets, std::map<int, std::vector<std::pair<int, int>>> animatedTiles) :
		tileSize(tileSize), tileSets(tileSets), animatedTiles(animatedTiles) {}

	virtual ~MapObject() = default;
	
	virtual void process(float deltaTime) {}
	virtual void draw(sf::RenderWindow& window) {}
	virtual void loadTexture() {}

	// Calculate x and y position of given tile in the texture
	void getTileCoords(sf::Texture *texture, int tile, int& x, int& y);
	int GetTextureIndex(int tileValue);

	const TileSize tileSize;
	std::map<int, sf::Texture *> &tileSets;
	std::map<int, std::vector<std::pair<int, int>>> animatedTiles;

	//possibly implement later, if decided to be useful
	//virtual void setPosition();
};

