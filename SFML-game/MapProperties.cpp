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

void MapProperties::saveProperties(Json::Value &properties) const
{
	for (const auto &[propertyName, anyObj] : m_propertyMap)
	{
		Json::Value value;
		value["name"] = propertyName;
		
		if (anyObj.type() == typeid(std::string))
		{
			if (auto castedValue = std::any_cast<std::string>(&anyObj))
			{
				value["value"] = *castedValue;
				value["type"] = "string";
			}
		}

		else if (anyObj.type() == typeid(std::filesystem::path))
		{
			if (auto castedValue = std::any_cast<std::filesystem::path>(&anyObj))
			{
				value["value"] = (*castedValue).string();
				value["type"] = "file";
			}
		}
		
		else if (anyObj.type() == typeid(sf::Color))
		{
			if (auto castedValue = std::any_cast<sf::Color>(&anyObj))
			{
				value["value"] = sf::utility::parseColor(*castedValue);
				value["type"] = "color";
			}
		}

		else if (anyObj.type() == typeid(int))
		{
			if (auto castedValue = std::any_cast<int>(&anyObj))
			{
				value["value"] = *castedValue;
				value["type"] = "int";
			}
		}

		else if (anyObj.type() == typeid(float))
		{
			if (auto castedValue = std::any_cast<float>(&anyObj))
			{
				value["value"] = *castedValue;
				value["type"] = "float";
			}
		}

		else if (anyObj.type() == typeid(bool))
		{
			if (auto castedValue = std::any_cast<bool>(&anyObj))
			{
				value["value"] = *castedValue;
				value["type"] = "bool";
			}
		}

		properties.append(value);
	}
}
