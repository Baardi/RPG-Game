#include "stdafx.h"
#include "sfUtility.hpp"
#include <string_view>
#include <sstream>
#include <numbers>

sf::Color sf::utility::parseColor(const std::string &colorCode)
{
	const size_t stringSize = colorCode.size();
	if (stringSize != 7  && stringSize != 9)
		return sf::Color::Black; // Default color is black
	
	const char *colorSumRgbHexStr = &*(colorCode.end() - 6);
	unsigned int colorSumRgb = std::stoul(colorSumRgbHexStr, nullptr, 16);

	unsigned int colorAlphaValue = 0xff;
	if (colorCode.length() == 9) // Tiled omits the value if alpha-value is 0xff
	{
		std::string colorAlpha(colorCode.begin() + 1, colorCode.begin() + 3);
		colorAlphaValue = std::stoul(colorAlpha.c_str(), nullptr, 16);
	}

	sf::Color color((colorSumRgb << 8) + colorAlphaValue);
	return color;
}

std::string sf::utility::serializeColor(sf::Color color)
{
	unsigned int colorSum = color.b + (color.g << 8) + (color.r << 16);
	if (color.a != 0xff)
		colorSum += (color.a << 24); // Tiled omits the value if alpha-value is 0xff

	std::stringstream stream;
	stream << "#" << std::hex << colorSum;

	return stream.str();
}

unsigned int sf::utility::parseTextStyle(const Json::Value &value)
{
	unsigned int style = Text::Regular;

	if (value["bold"].asBool())      style |= Text::Bold;
	if (value["italic"].asBool())    style |= Text::Italic;
	if (value["strikeout"].asBool()) style |= Text::StrikeThrough;
	if (value["underline"].asBool()) style |= Text::Underlined;

	return style;
}

Json::Value sf::utility::serializeTextStyle(unsigned int style)
{
	Json::Value value;

	if (style & Text::Bold)					 value["bold"]			= true;
	if (style & Text::Italic)				 value["italic"]		= true;
	if (style & Text::StrikeThrough) value["strikeout"]	= true;
	if (style & Text::Underlined)		 value["underline"]	= true;

	return value;
}

sf::Transform sf::utility::computeTransform(sf::Vector2f origin, sf::Vector2f translation, sf::Vector2f scale, float rotation)
{
	const float theta = -rotation * std::numbers::pi_v<float> / 180.f;
	const float sxc = scale.x * std::cos(theta);
	const float syc = scale.y * std::cos(theta);
	const float sxs = scale.x * std::sin(theta);
	const float sys = scale.y * std::sin(theta);
	const float tx = -origin.x * sxc - origin.y * sys + translation.x;
	const float ty =  origin.x * sxs - origin.y * syc + translation.y;

	return Transform(sxc, sys, tx,
									-sxs, syc, ty,
									0.f, 0.f, 1.f);
}
