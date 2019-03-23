#pragma once
#include "Layer.hpp"
#include "MapProperties.hpp"

// Class representing a tile layer
class TileLayer : public Layer, public MapProperties
{
public:
	TileLayer(const TileSize& tileSize);
	~TileLayer();

	void load(const Json::Value& layer, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles);
	void draw(sf::RenderTarget& window) override;
	void process(const sftools::Chronometer &clock) override;

	// Safe to use from map, safe in case of breaking the bounds of the layer array
	bool containsTexture(double x, double y) const;

private:
	void initArrays(int size); // Resizes vector according to width/height of layer
	
	void loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles) override;
	void loadSpriteTexture(sf::Texture &texture, int tileid, int x, int y);
	void loadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, sf::Time>> &animationTile, int x, int y);
		
	bool containsTextureTileCoords(int x, int y) const;
	
	// Should only be used internally, as there's no boundary check
	template <class T>
	T& getValue(std::vector<T> &arr, int x, int y);
	template <class T>
	const T& getValue(const std::vector<T> &arr, int x, int y) const;

    // Use get (array, x, y) to access the map
    std::vector<int> m_tilemap;
	std::vector<AnimationTile> m_animationTilemap;
	std::vector<sf::Sprite> m_textureMap;

    // Size in tiles
	int m_width, m_height;
};
