#pragma once

#include "Layer.hpp"
#include <filesystem>
#include <optional>

class ImageLayer : public Layer
{
public:
	ImageLayer(TileSize tileSize) : Layer(tileSize) {}
	~ImageLayer() = default;

	void load(const Json::Value &layer, const std::filesystem::path &directory, std::map<std::string, sf::Texture> &textures);
	void save(Json::Value &layers) const;
	void draw(sf::RenderTarget &target);

	std::filesystem::path image;
	sf::Sprite m_sprite;
	int x, y;
	std::optional<sf::Color> transparentcolor;
};
