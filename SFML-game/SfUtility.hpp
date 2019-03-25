#pragma once

namespace sf::utility
{
	Color parseColor(const std::string &colorCode);
	unsigned int parseTextStyle(const Json::Value &value);
	sf::Transform computeTransform(sf::Vector2f origin, sf::Vector2f translation, sf::Vector2f scale, float rotation);
}
