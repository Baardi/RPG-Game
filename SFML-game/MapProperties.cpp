#include "stdafx.h"
#include "MapProperties.h"
#include "sfutility.h"
#include <filesystem>

void MapProperties::loadProperties(const Json::Value &object)
{
	auto &propertytypes = object["propertytypes"];
	auto &properties = object["properties"];
	for (auto it = properties.begin(); it != properties.end(); ++it)
	{
		auto propertyName = it.key().asString();
		auto propertyType = propertytypes[propertyName].asString();

		if (propertyType == "string")
			m_propertyMap.try_emplace(propertyName, std::make_any<std::string>(it->asString()));

		else if (propertyType == "file")
			m_propertyMap.try_emplace(propertyName, std::make_any<std::filesystem::path>(it->asString()));

		else if (propertyType == "color")
			m_propertyMap.try_emplace(propertyName, std::make_any<sf::Color>(sf::utility::parseColor(it->asString())));

		else if (propertyType == "int")
			m_propertyMap.try_emplace(propertyName, std::make_any<int>(it->asInt()));

		else if (propertyType == "float")
			m_propertyMap.try_emplace(propertyName, std::make_any<float>(it->asFloat()));

		else if (propertyType == "bool")
			m_propertyMap.try_emplace(propertyName, std::make_any<bool>(it->asBool()));
	}
}
