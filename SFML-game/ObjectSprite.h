#pragma once
#include "Layer.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "GameObject.h"

class ObjectSprite : public Layer, public GameObject
{
	friend class Map;
	friend class ObjectLayer;
public:
	ObjectSprite(const TileSize &tileSize, std::unordered_map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles, sf::Clock &clock) : Layer(tileSize, tileSets, animatedTiles), clock(clock) {}
	~ObjectSprite() = default;

	void process() override;
	void draw(sf::RenderWindow& window) override;
	void loadTexture() override;

	sf::FloatRect GetGlobalBounds() override;
	std::string GetPropertyValue(const std::string &propertyName);
	void LoadSpriteTexture(sf::Texture &texture, int tileid);
	void LoadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, int>> &animationTile);
	sf::IntRect GetTextureRectToUse(int tilex, int tiley, bool verflip = false, bool horflip = false);

private:

	// Id of first tile
	int gid;

	// Location on screen
	float x, y;
	float width, height;
	float rotation;
	bool horflip, verflip;

	// AnimationData
	AnimationTile animationTileInfo;

	// Times the animation
	sf::Clock &clock;

	sf::Sprite sprite;
	sf::FloatRect globalBounds;  // May need a specifier for how to get GlobalBounds (via sprite or via x/y/width/height)

	std::unordered_map<std::string, std::string> propertyMap;
};
