#pragma once

namespace sf::utility
{
	Color parseColor(const std::string &colorCode);
	std::string serializeColor(sf::Color color);

	unsigned int parseTextStyle(const Json::Value &value);
	Json::Value serializeTextStyle(unsigned int style);
	
	Transform computeTransform(Vector2f origin, Vector2f translation, Vector2f scale, float rotation);
}
