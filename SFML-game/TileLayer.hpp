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

	bool load(const Json::Value& layer, const std::unordered_map<int, TileSet> &tileSets);
	bool save(Json::Value &layers) const override;

	void draw(sf::RenderTarget &target) override;
	void drawWithFringe(sf::RenderTarget& target, FringeDrawer &fringeDrawer) override;
	void process(const sftools::Chronometer &clock) override;

	// Safe to use from map, safe in case of breaking the bounds of the layer array
	bool containsTexture(sf::Vector2f) const;

private:
	void loadTexture(const std::unordered_map<int, TileSet> &tileSets) override;
	void loadSpriteTexture(Tile& tile, const sf::Texture& texture, int tileid, sf::Vector2i);
	void loadSpriteAnimation(const sf::Texture &texture, Tile &tile, const Animation &animation);
		
	bool containsTextureTileCoords(sf::Vector2i pos) const;

    // Layer representation
    std::vector<Tile> m_tilemap;
	int width, height;
};
