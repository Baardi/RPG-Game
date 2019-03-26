#include "stdafx.h"
#include "MapProperties.hpp"
#include "sfUtility.hpp"
#include <filesystem>

void MapProperties::loadProperties(const Json::Value &properties)
{
	for (auto &property : properties)
	{
		auto propertyName = property["name"].asString();
		auto propertyType = property["type"].asString();
		auto propertyValue = property["value"];

		if (propertyType == "string")
			m_propertyMap.try_emplace(propertyName, std::make_any<std::string>(propertyValue.asString()));

		else if (propertyType == "file")
			m_propertyMap.try_emplace(propertyName, std::make_any<std::filesystem::path>(propertyValue.asString()));

		else if (propertyType == "color")
			m_propertyMap.try_emplace(propertyName, std::make_any<sf::Color>(sf::utility::parseColor(propertyValue.asString())));

		else if (propertyType == "int")
			m_propertyMap.try_emplace(propertyName, std::make_any<int>(propertyValue.asInt()));

		else if (propertyType == "float")
			m_propertyMap.try_emplace(propertyName, std::make_any<float>(propertyValue.asFloat()));

		else if (propertyType == "bool")
			m_propertyMap.try_emplace(propertyName, std::make_any<bool>(propertyValue.asBool()));
	}
}
