#pragma once
#include <any>
#include "Layer.h"
#include "GameObject.h"

class ObjectSprite : public Layer, public GameObject
{
	friend class Map;
	friend class ObjectLayer;
public:
	ObjectSprite(const TileSize &tileSize, std::map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles, sftools::Chronometer &clock) : Layer(tileSize, tileSets, animatedTiles), clock(clock) {}
	~ObjectSprite() = default;

	void process() override;
	void draw(sf::RenderWindow& window) override;
	void loadTexture() override;

	sf::FloatRect GetGlobalBounds() override;

	template <class T>
	T GetProperty(const std::string &propertyName)
	{
		return std::any_cast<T>(propertyMap.find(propertyName)->second);
	}
	void LoadSpriteTexture(sf::Texture &texture, int tileid);
	void LoadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, int>> &animationTile);
	sf::IntRect GetTextureRectToUse(int tilex, int tiley, bool verflip = false, bool horflip = false) const;

private:

	// Id of first tile
	int gid;

	// Location on screen
	float x, y;
	float width, height;
	float rotation;
	bool horflip, verflip;

	std::unique_ptr<sf::Text> text;

	// AnimationData
	AnimationTile animationTileInfo;

	// Times the animation
	sftools::Chronometer &clock;

	sf::Sprite sprite;
	sf::FloatRect globalBounds;  // May need a specifier for how to get GlobalBounds (via sprite or via x/y/width/height)

	std::map<std::string, std::any> propertyMap;
};
