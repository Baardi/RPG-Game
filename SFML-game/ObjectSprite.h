#pragma once
#include <any>
#include "Layer.h"
#include "GameObject.h"
#include "MapProperties.h"
#include <optional>

class ObjectSprite : public Layer, public GameObject, public MapProperties
{
public:
	ObjectSprite(const TileSize &tileSize, std::map<int, sf::Texture *> &tileSets, AnimationTileMap &animatedTiles, sftools::Chronometer &clock) : Layer(tileSize, tileSets, animatedTiles), clock(clock) {}
	~ObjectSprite() = default;

	void load(Json::Value &layer, Json::Value &object);
	void loadText(Json::Value &textValue);
	void process() override;
	void draw(sf::RenderTarget& window) override;
	void loadTexture() override;

	sf::DoubleRect GetGlobalBounds() const override;

	void LoadSpriteTexture(sf::Texture &texture, int tileid);
	void LoadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, int>> &animationTile);
	sf::IntRect GetTextureRectToUse(int tilex, int tiley, bool verflip = false, bool horflip = false) const;

	// Id of first tile
	int gid;

	sf::Sprite sprite;
private:

	// Location on screen
	float x, y;
	float width, height;
	float rotation;
	bool horflip, verflip;

	std::optional<sf::Text> text;

	// AnimationData
	AnimationTile animationTileInfo;

	// Times the animation
	sftools::Chronometer &clock;

	sf::DoubleRect globalBounds;  // May need a specifier for how to get GlobalBounds (via sprite or via x/y/width/height)
};
