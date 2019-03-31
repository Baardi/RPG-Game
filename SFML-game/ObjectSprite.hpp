#pragma once
#include <any>
#include "TileSet.hpp"
#include "GameObject.hpp"
#include "MapProperties.hpp"
#include <optional>
#include "ObjectFactory.hpp"

class ObjectSprite : public MapProperties, public GameObject
{
public:
	ObjectSprite() = default;
	virtual ~ObjectSprite() = default;

	virtual void applyProperties() {}

	void load(const Json::Value &layer, const Json::Value &object, const std::map<int, TileSet> &tileSets);
	void loadText(const Json::Value &textValue);
	void save(Json::Value &objects) const;
	void saveText(Json::Value &textValue) const;

	void process(const sftools::Chronometer &clock);
	void draw(sf::RenderTarget &target) override;
	
	void loadTexture(const std::map<int, TileSet> &tileSets);

	sf::FloatRect getLocalBounds() const override;
	sf::Transform getTransform() const override;

	// Id of first tile
	int gid;
	sf::Sprite sprite;
	std::string name;
	std::string type;
	bool visible;

	// Tileset used by sprite
	const TileSet *tileset;

	// Location on screen
	float x, y;
	float width, height;
	float rotation;
	bool horflip, verflip;
	float opacity;
	int id;
	
	std::optional<sf::Text> text;

protected:
	void loadSpriteTexture(const sf::Texture &texture, int tileid);
	void loadSpriteAnimation(const sf::Texture &texture, const std::vector<std::pair<int, sf::Time>> &animationTile);
	sf::IntRect getTextureRectToUse(int tilex, int tiley, bool verflip = false, bool horflip = false) const;

private:

	// AnimationData
	AnimationTile m_animationTileInfo;

	sf::Transform m_transform;
	sf::FloatRect m_localBounds;
};

using ObjectSpriteFactory = ObjectFactory<ObjectSprite>;
