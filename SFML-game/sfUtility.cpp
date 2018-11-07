#include "stdafx.h"
#include "sfUtility.h"

sf::Color sf::utility::parseColor(const std::string &colorCode)
{
	// A really lame method that parses color codes from tiled into sf::color.
	// Throws exception if the length isn't 7 or 9
	auto stringSize = colorCode.size();
	if (stringSize != 7  && stringSize != 9)
		throw;

	char buf[2]; // Set up buffer for color components
	
	buf[0] = colorCode[stringSize - 6];
	buf[1] = colorCode[stringSize - 5];
	auto red = std::stoi(buf, nullptr, 16);		// Copy red component

	buf[0] = colorCode[stringSize - 4];
	buf[1] = colorCode[stringSize - 3];
	auto green = std::stoi(buf, nullptr, 16);	// Copy green component

	buf[0] = colorCode[stringSize - 2];
	buf[1] = colorCode[stringSize - 1];
	auto blue = std::stoi(buf, nullptr, 16);	// Copy blue component

	auto alpha = 255;
	if (stringSize == 9)
	{
		buf[0] = colorCode[1];
		buf[1] = colorCode[2];
		alpha = std::stoi(buf, nullptr, 16);	// Copy alpha component
	}

	return Color(red, green, blue, alpha);		// Construct the color
}

unsigned int sf::utility::parseTextStyle(const Json::Value &value)
{
	// Todo: test possibility to throw if error

	unsigned int style = Text::Regular;

	if (value["bold"].asBool())      style |= Text::Bold;
	if (value["italic"].asBool())    style |= Text::Italic;
	if (value["strikeout"].asBool()) style |= Text::StrikeThrough;
	if (value["underline"].asBool()) style |= Text::Underlined;

	return style;
}
