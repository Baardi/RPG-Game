#pragma once
#include "Layer.h"

// Class representing a tile layer
class TileLayer : public Layer
{
	// Map needs to access protected/private data
	friend class Map;

public:
	TileLayer(const TileSize& tileSize, std::unordered_map<int, sf::Texture *>& tileSets, AnimationTileMap& animatedTiles, sf::Clock& clock);
	~TileLayer();

	void draw(sf::RenderWindow& window) override;
	void process() override;

	// Safe to use from map, safe in case of breaking the bounds of the layer array
	bool containsTexture(double x, double y) const;

private:
	void initArrays(); // Allocates memory according to width/height of layer
	
	void loadTexture() override;
	void LoadSpriteTexture(sf::Texture &texture, int tileid, int x, int y);
	void LoadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, int>> &animationTile, int x, int y);
		
	bool containsTextureTileCoords(int x, int y) const;
	
	// Should only be used internally, as there's no boundary check
	template <class T>
	static T& get(T* arr, int x, int y);

    // Use get (array, x, y) to access the map
    int *tilemap;
	AnimationTile *animationTilemap;
	sf::Sprite *textureMap;

    // Size in tiles
	int width, height;

	sf::Clock &clock;
};
