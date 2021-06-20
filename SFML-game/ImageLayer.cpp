#include "stdafx.h"
#include "ImageLayer.hpp"
#include "SfUtility.hpp"

void ImageLayer::load(const Json::Value &layer, const std::filesystem::path &directory, std::map<std::string, sf::Texture>& textures)
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
		
	m_sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
	m_sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(256 * opacity) - 1));

	auto imagePath = directory / image;
	auto it = textures.find(imagePath.string());
	if (it == textures.end())
	{
		auto[it2, inserted] = textures.try_emplace(imagePath.string(), sf::Texture());
		if (inserted)
		{
			it2->second.loadFromFile(imagePath.string());
			m_sprite.setTexture(it2->second);
		}
	}
	else
		m_sprite.setTexture(it->second);
}

void ImageLayer::save(Json::Value &layers) const
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
}

void ImageLayer::draw(sf::RenderTarget &target)
{
	if (visible)
		target.draw(m_sprite);
}
