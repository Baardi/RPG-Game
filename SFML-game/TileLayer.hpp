#pragma once
#include "Layer.hpp"

struct Tile
{
	int id;
	AnimationTile animation;
	sf::Sprite sprite;
	const TileSet *tileset;
};

// Class representing a tile layer
class TileLayer : public Layer
{
public:
	TileLayer(TileSize tileSize) : Layer(tileSize) {}
	~TileLayer() = default;

	void load(const Json::Value& layer, const std::map<int, TileSet> &tileSets);
	void save(Json::Value &layers) const override;

	void draw(sf::RenderTarget &target) override;
	void process(const sftools::Chronometer &clock) override;

	// Safe to use from map, safe in case of breaking the bounds of the layer array
	bool containsTexture(double x, double y) const;

private:
	void loadTexture(const std::map<int, TileSet> &tileSets) override;
	void loadSpriteTexture(Tile &tile, const sf::Texture &texture, int tileid, int x, int y);
	void loadSpriteAnimation(const sf::Texture &texture, Tile &tile, const std::vector<std::pair<int, sf::Time>> &animationTile);
		
	bool containsTextureTileCoords(int x, int y) const;

    // Layer representation
    std::vector<Tile> m_tilemap;
	int width, height;
};
