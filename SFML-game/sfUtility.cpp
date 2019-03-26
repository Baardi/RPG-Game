#include "stdafx.h"
#include "sfUtility.hpp"

sf::Color sf::utility::parseColor(const std::string &colorCode)
{
	// A really lame method that parses color codes from tiled into sf::color.
	// Throws exception if the length isn't 7 or 9
	auto stringSize = colorCode.size();
	if (stringSize != 7  && stringSize != 9)
		throw std::runtime_error("Yikes");

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

	Color color(red, green, blue, alpha);
	return color;
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

sf::Transform sf::utility::computeTransform(sf::Vector2f origin, sf::Vector2f translation, sf::Vector2f scale, float rotation)
{
	float angle = -rotation * 3.141592654f / 180.f;
	float cosine = static_cast<float>(std::cos(angle));
	float sine = static_cast<float>(std::sin(angle));
	float sxc = scale.x * cosine;
	float syc = scale.y * cosine;
	float sxs = scale.x * sine;
	float sys = scale.y * sine;
	float tx = -origin.x * sxc - origin.y * sys + translation.x;
	float ty = origin.x * sxs - origin.y * syc + translation.y;

	Transform transform(sxc, sys, tx,
		-sxs, syc, ty,
		0.f, 0.f, 1.f);

	return transform;
}
