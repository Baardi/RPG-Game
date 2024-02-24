#include "stdafx.h"
#include "ImageLayer.hpp"
#include "SfUtility.hpp"
#include <print>

bool ImageLayer::load(const Json::Value &layer, const std::filesystem::path &directory, Textures &textures)
{
	type = layer["type"].asString();
	image = layer["image"].asString();
	name = layer["name"].asString();
	x = layer["x"].asInt();
	y = layer["y"].asInt();
	visible = layer["visible"].asBool();
	opacity = layer["opacity"].asFloat();
	id = layer["id"].asInt();

	if (!layer["transparentcolor"].empty())
	{
		transparentcolor = sf::utility::parseColor(layer["transparentcolor"].asString());
	}


	const auto imagePath = directory / image;
	const auto [it, inserted] = textures.try_emplace(imagePath);
	if (inserted)
	{
		if (!it->second.loadFromFile(imagePath))
		{
			std::println("Failed loading texture");

			return false;
		}
	}

	m_sprite.emplace(it->second);
	m_sprite->setPosition(sf::Vector2f{ static_cast<float>(x), static_cast<float>(y) });
	m_sprite->setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(256 * opacity) - 1));

	return true;
}

bool ImageLayer::save(Json::Value &layers) const
{
	Json::Value layer;

	layer["type"] = "imagelayer";
	layer["image"] = image.string();
	layer["name"] = name;
	layer["x"] = x;
	layer["x"] = y;
	layer["visible"] = visible;
	layer["opacity"] = opacity;
	layer["id"] = id;

	if (transparentcolor.has_value())
		layer["transparentcolor"] = sf::utility::serializeColor(*transparentcolor);
	
	layers.append(layer);

	return true;
}

void ImageLayer::draw(sf::RenderTarget &target)
{
	if (visible && m_sprite)
		target.draw(*m_sprite);
}
