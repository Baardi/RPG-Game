#pragma once
#include "Layer.hpp"

struct Tile
{
	int id;
	AnimationTile animation;
	sf::Sprite sprite;
};

// Class representing a tile layer
class TileLayer : public Layer
{
public:
	TileLayer(const TileSize& tileSize);
	~TileLayer();

	void load(const Json::Value& layer, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles);
	void save(Json::Value &layers) const override;

	void draw(sf::RenderTarget& window) override;
	void process(const sftools::Chronometer &clock) override;

	// Safe to use from map, safe in case of breaking the bounds of the layer array
	bool containsTexture(double x, double y) const;

private:
	void loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles) override;
	void loadSpriteTexture(sf::Sprite &sprite, const sf::Texture &texture, int tileid, int x, int y);
	void loadSpriteAnimation(const sf::Texture &texture, Tile &tile, std::vector<std::pair<int, sf::Time>> &animationTile);
		
	bool containsTextureTileCoords(int x, int y) const;

    // Layer representation
    std::vector<Tile> m_tilemap;
	int width, height;
};
