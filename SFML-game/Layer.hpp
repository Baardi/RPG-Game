#pragma once
#include "MapProperties.hpp"

class TileSet;
class FringeDrawer;

// <animationtileid, animationdata< frame<tileid, duration>> >
using AnimationTileMap = std::map<int, std::vector< std::pair<int, sf::Time>>>;

struct AnimationTileData
{
	AnimationTileData(sf::Time duration, sf::IntRect intRect) :
		duration(duration), intRect(intRect) {}

	sf::Time duration;
	sf::IntRect intRect;
};

struct AnimationTile
{
	int currentFrame = 0;
	sf::Time lastTime;
	std::vector<AnimationTileData> data;
};

// Small helper struct that contains tile size information
struct TileSize
{
	TileSize() : x(0), y(0), s(0), m(0) {}
	int x; // Width
	int y; // Height
	int s; // Spacing
	int m; // Margin
};

class Layer : public MapProperties
{
public:
	Layer(TileSize tileSize) : tileSize(tileSize) {}
	virtual ~Layer() = default;
	
	virtual void save(Json::Value &layer) const {}
	virtual void process(const sftools::Chronometer &clock) {}
	virtual void draw(sf::RenderTarget& target) {}
	virtual void drawWithFringe(sf::RenderTarget& target, FringeDrawer &fringeDrawer) { draw(target); }
	virtual void loadTexture(const std::map<int, TileSet> &tileSets) {}

	std::string name;
	bool visible;
	std::string type;
	float opacity;
	int id;
	TileSize tileSize;
};

int getTextureIndex(int tileValue, const std::map<int, TileSet> &tileSets);
void processAnimation(sf::Sprite &sprite, AnimationTile &animationTile, const sftools::Chronometer& clock);	
sf::Vector2i getTileCoords(const sf::Texture &texture, int tile, TileSize tileSize);
