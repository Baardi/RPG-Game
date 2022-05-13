#pragma once

#include "Layer.hpp"
#include <filesystem>
#include <optional>
#include "App/ResourceManager.hpp"

class ImageLayer : public Layer
{
public:
	ImageLayer(TileSize tileSize) : Layer(tileSize), x(0), y(0) {}
	~ImageLayer() = default;

	bool load(const Json::Value &layer, const std::filesystem::path &directory, Textures &textures);
	bool save(Json::Value &layers) const;
	void draw(sf::RenderTarget &target);

	std::filesystem::path image;
	sf::Sprite m_sprite;
	int x, y;
	std::optional<sf::Color> transparentcolor;
};
