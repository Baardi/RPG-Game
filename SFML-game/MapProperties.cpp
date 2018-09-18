#include "stdafx.h"
#include "MapProperties.h"
#include "utility.h"
#include <filesystem>

void MapProperties::LoadProperties(Json::Value &object)
{
	auto &propertytypes = object["propertytypes"];
	auto &properties = object["properties"];
	for (Json::ValueIterator it = properties.begin(); it != properties.end(); ++it)
	{
		auto propertyName = it.key().asString();
		auto propertyType = propertytypes[propertyName].asString();

		if (propertyType == "string")
			propertyMap.try_emplace(propertyName, std::make_any<std::string>(it->asString()));

		else if (propertyType == "file")
			propertyMap.try_emplace(propertyName, std::make_any<std::filesystem::path>(it->asString()));

		else if (propertyType == "color")
			propertyMap.try_emplace(propertyName, std::make_any<sf::Color>(sf::utility::parseColor(it->asString())));

		else if (propertyType == "int")
			propertyMap.try_emplace(propertyName, std::make_any<int>(it->asInt()));

		else if (propertyType == "float")
			propertyMap.try_emplace(propertyName, std::make_any<float>(it->asFloat()));

		else if (propertyType == "bool")
			propertyMap.try_emplace(propertyName, std::make_any<bool>(it->asBool()));
	}
}
