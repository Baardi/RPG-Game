#pragma once

using AnimationTileMap = std::map<int, std::vector< std::pair<int, int>> >;

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
public:
	Layer(const TileSize &tileSize, std::map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles) :
		tileSize(tileSize), tileSets(tileSets), animatedTiles(animatedTiles) {}

	virtual ~Layer() = default;
	
	virtual void process() {}
	virtual void draw(sf::RenderTarget& window) {}
	virtual void loadTexture() {}

	std::string name;
	bool visible;
protected:

	// Calculate x and y position of given tile in the texture
	void getTileCoords(sf::Texture *texture, int tile, int& x, int& y) const;
	int GetTextureIndex(int tileValue) const;
	static void ProcessAnimation(sf::Sprite &sprite, AnimationTile &animationTile, sftools::Chronometer& clock);
	
	const TileSize &tileSize;
	std::map<int, sf::Texture *> &tileSets;
	AnimationTileMap &animatedTiles;

	std::string type;
	float opacity;
};

void g_getTileCoords(sf::Texture *texture, int tile, int& x, int& y, const TileSize tileSize);
