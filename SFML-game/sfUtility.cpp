#include "stdafx.h"
#include "sfUtility.hpp"

sf::Color sf::utility::parseColor(const std::string &colorCode)
{
	auto stringSize = colorCode.size();
	if (stringSize != 7  && stringSize != 9)
		throw std::runtime_error("Wrong string length. Can't parse color");
	
	std::string colorRgb(colorCode.end() - 6, colorCode.end());
	unsigned int colorSumRgb = std::stoul(colorRgb.c_str(), nullptr, 16);

	unsigned int colorSumAlpha = 255;
	if (colorCode.length() == 9) // Tiled omits the value if alpha-value is 0xff
	{
		std::string colorAlpha(colorCode.begin() + 1, colorCode.begin() + 3);
		colorSumAlpha = std::stoul(colorAlpha.c_str(), nullptr, 16);
	}

	Color color((colorSumRgb << 8) + colorSumAlpha);
	return color;
}

std::string sf::utility::parseColor(sf::Color color)
{
	unsigned int colorSum = color.b + (color.g << 8) + (color.r << 16);
	if (color.a != 255)
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

Json::Value sf::utility::parseTextStyle(unsigned int style)
{
	Json::Value value;

	if (style & Text::Bold)			value["bold"] = true;
	if (style & Text::Italic)		value["italic"] = true;
	if (style & Text::StrikeThrough)value["strikeout"] = true;
	if (style & Text::Underlined)	value["underline"] = true;

	return value;
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
