#include "stdafx.h"
#include "Utility.h"

sf::Color sf::Utility::parseColor(const std::string &colorCode)
{
	// A really lame method that parses color codes from tiled into sf::color.
	// Throws exception if the length isn't 7 or 9
	if (colorCode.size() != 7  && colorCode.size() != 9)
		throw;

	char buf[2]; // Set up buffer for color components
	
	buf[0] = colorCode[colorCode.size() - 6];
	buf[1] = colorCode[colorCode.size() - 5];
	auto red = std::stoi(buf, nullptr, 16);		// Copy red component

	buf[0] = colorCode[colorCode.size() - 4];
	buf[1] = colorCode[colorCode.size() - 3];
	auto green = std::stoi(buf, nullptr, 16);	// Copy green component

	buf[0] = colorCode[colorCode.size() - 2];
	buf[1] = colorCode[colorCode.size() - 1];
	auto blue = std::stoi(buf, nullptr, 16);	// Copy blue component

	auto alpha = 255;
	if (colorCode.size() == 9)
	{
		buf[0] = colorCode[1];
		buf[1] = colorCode[2];
		alpha = std::stoi(buf, nullptr, 16);	// Copy alpha component
	}

	return Color(red, green, blue, alpha);			// Constructor the color
}
