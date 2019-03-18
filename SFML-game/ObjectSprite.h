#pragma once
#include <any>
#include "Layer.h"
#include "GameObject.h"
#include "MapProperties.h"
#include <optional>

class ObjectSprite : public Layer, public GameObject, public MapProperties
{
public:
	ObjectSprite(const TileSize &tileSize) : Layer(tileSize) {}
	~ObjectSprite() = default;

	void load(const Json::Value &layer, const Json::Value &object, std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles);
	void loadText(const Json::Value &textValue);
	void process(sftools::Chronometer &clock) override;
	void draw(sf::RenderTarget& window) override;
	void loadTexture(std::map<int, sf::Texture*>& tileSets, AnimationTileMap &animatedTiles) override;

	sf::FloatRect GetGlobalBounds() const override;

	// Id of first tile
	int gid;

	sf::Sprite sprite;

protected:
	void LoadSpriteTexture(sf::Texture &texture, int tileid);
	void LoadSpriteAnimation(sf::Texture &texture, std::vector<std::pair<int, int>> &animationTile);
	sf::IntRect GetTextureRectToUse(int tilex, int tiley, bool verflip = false, bool horflip = false) const;

private:
	// Location on screen
	float x, y;
	float width, height;
	float rotation;
	bool horflip, verflip;

	std::optional<sf::Text> text;

	// AnimationData
	AnimationTile animationTileInfo;
	
	sf::FloatRect globalBounds;  // May need a specifier for how to get GlobalBounds (via sprite or via x/y/width/height)
};
