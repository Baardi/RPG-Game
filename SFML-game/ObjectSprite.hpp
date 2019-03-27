#pragma once
#include <any>
#include "TileSet.hpp"
#include "GameObject.hpp"
#include "MapProperties.hpp"
#include <optional>
#include "ObjectFactory.hpp"

class ObjectSprite : public Layer, public GameObject
{
public:
	ObjectSprite(const TileSize &tileSize) : Layer(tileSize) {}
	~ObjectSprite() = default;

	void load(const Json::Value &layer, const Json::Value &object, const std::map<int, TileSet> &tileSets);
	void loadText(const Json::Value &textValue);
	void save(Json::Value &objects) const override;
	void saveText(Json::Value &textValue) const;

	void process(const sftools::Chronometer &clock) override;
	void draw(sf::RenderTarget &target) override;
	
	void loadTexture(const std::map<int, TileSet> &tileSets) override;

	sf::FloatRect getLocalBounds() const override;
	sf::Transform getTransform() const override;

	// Id of first tile
	int gid;

	sf::Sprite sprite;

protected:
	void loadSpriteTexture(const sf::Texture &texture, int tileid);
	void loadSpriteAnimation(const sf::Texture &texture, const std::vector<std::pair<int, sf::Time>> &animationTile);
	sf::IntRect getTextureRectToUse(int tilex, int tiley, bool verflip = false, bool horflip = false) const;

private:
	// Location on screen
	float x, y;
	float width, height;
	float rotation;
	bool horflip, verflip;

	std::optional<sf::Text> text;

	// AnimationData
	AnimationTile m_animationTileInfo;

	sf::Transform m_transform;
	sf::FloatRect m_localBounds;
};

using ObjectSpriteFactory = ObjectFactory<ObjectSprite, const TileSize &>;
