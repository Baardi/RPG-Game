#pragma once
#include "Layer.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>

class ObjectSprite : public Layer
{
	friend class Map;
	friend class ObjectLayer;
public:
	ObjectSprite(TileSize tileSize, std::unordered_map<int, sf::Texture *> &tileSets, std::unordered_map<int, std::vector<std::pair<int, int>>> animatedTiles) : Layer(tileSize, tileSets, animatedTiles) {}
	~ObjectSprite() = default;

	void process() override;
	void draw(sf::RenderWindow& window) override;
	void loadTexture() override;


protected:

	// Id of first tile
	int gid;

	// Location on screen
	float x, y;
	float width, height;
	float rotation;

	// AnimationData
	AnimationTile animationTileInfo;

	// Times the animation
	sf::Clock clock;

	sf::Sprite sprite;
};
