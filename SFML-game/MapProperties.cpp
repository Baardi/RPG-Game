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
			m_propertyMap.try_emplace(propertyName, std::make_pair("string", std::make_any<std::string>(propertyValue.asString())));

		else if (propertyType == "file")
			m_propertyMap.try_emplace(propertyName, std::make_pair("file", std::make_any<std::filesystem::path>(propertyValue.asString())));

		else if (propertyType == "color")
			m_propertyMap.try_emplace(propertyName, std::make_pair("color", std::make_any<sf::Color>(sf::utility::parseColor(propertyValue.asString()))));

		else if (propertyType == "int")
			m_propertyMap.try_emplace(propertyName, std::make_pair("int", std::make_any<int>(propertyValue.asInt())));

		else if (propertyType == "float")
			m_propertyMap.try_emplace(propertyName, std::make_pair("float", std::make_any<float>(propertyValue.asFloat())));

		else if (propertyType == "bool")
			m_propertyMap.try_emplace(propertyName, std::make_pair("bool", std::make_any<bool>(propertyValue.asBool())));
	}
}

void MapProperties::saveProperties(Json::Value &properties) const
{
	for (const auto[propertyName, pair] : m_propertyMap)
	{
		auto[type, anyObj] = pair;

		Json::Value value;
		value["name"] = propertyName;
		value["type"] = type;
		
		if (type == "string")
		{
			auto castedValue = std::any_cast<std::string>(&anyObj);
			if (castedValue)
				value["value"] = *castedValue;
		}

		else if (type == "file")
		{
			auto castedValue = std::any_cast<std::filesystem::path>(&anyObj);
			if (castedValue)
				value["value"] = (*castedValue).string();
		}
		
		else if (type == "color")
		{
			auto castedValue = std::any_cast<sf::Color>(&anyObj);
			if (castedValue)
				value["value"] = sf::utility::parseColor(*castedValue);
		}

		else if (type == "int")
		{
			auto castedValue = std::any_cast<int>(&anyObj);
			if (castedValue)
				value["value"] = *castedValue;
		}

		else if (type == "float")
		{
			auto castedValue = std::any_cast<float>(&anyObj);
			if (castedValue)
				value["value"] = *castedValue;
		}

		else if (type == "bool")
		{
			auto castedValue = std::any_cast<bool>(&anyObj);
			if (castedValue)
				value["value"] = *castedValue;
		}

		if (!value.empty())
			properties.append(value);
	}
}
