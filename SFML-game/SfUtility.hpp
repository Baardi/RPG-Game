#pragma once

namespace sf::utility
{
	Color parseColor(const std::string &colorCode);
	unsigned int parseTextStyle(const Json::Value &value);
	Transform computeTransform(Vector2f origin, Vector2f translation, Vector2f scale, float rotation);
}
