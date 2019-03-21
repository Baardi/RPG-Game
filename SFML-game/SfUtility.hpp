#pragma once

namespace sf::utility
{
	Color parseColor(const std::string &colorCode);
	unsigned int parseTextStyle(const Json::Value &value);
}
