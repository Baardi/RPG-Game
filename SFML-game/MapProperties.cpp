#include "stdafx.h"
#include "MapProperties.hpp"
#include "sfUtility.hpp"

void MapProperties::loadProperties(const Json::Value &properties)
{
	for (const auto &property : properties)
	{
		const auto propertyName = property["name"].asString();
		const auto propertyType = property["type"].asString();
		const auto propertyValue = property["value"];

		if (propertyType == "string")
			m_properties[propertyName] = propertyValue.asString();
		else if (propertyType == "file")
			m_properties[propertyName] = std::filesystem::path{ propertyValue.asString() };
		else if (propertyType == "color")
			m_properties[propertyName] = sf::utility::parseColor(propertyValue.asString());
		else if (propertyType == "int")
			m_properties[propertyName] = propertyValue.asInt();
		else if (propertyType == "float")
			m_properties[propertyName] = propertyValue.asFloat();
		else if (propertyType == "bool")
			m_properties[propertyName] = propertyValue.asBool();
	}
}

void MapProperties::saveProperties(Json::Value &properties) const
{
	for (const auto &[propertyName, propertyValue] : m_properties)
	{
		Json::Value value;
		value["name"] = propertyName;
		value["type"] = std::visit([](auto &&arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::string>)
				return "string";
			else if constexpr (std::is_same_v<T, std::filesystem::path>)
				return "file";
			else if constexpr (std::is_same_v<T, sf::Color>)
				return "color";
			else if constexpr (std::is_same_v<T, int>)
				return "int";
			else if constexpr (std::is_same_v<T, float>)
				return "float";
			else if constexpr (std::is_same_v<T, bool>)
				return "bool";
			}, propertyValue);
		
		if (const auto stringValue = std::get_if<std::string>(&propertyValue))
			value["value"] = *stringValue;
		else if (const auto filePathValue = std::get_if<std::filesystem::path>(&propertyValue))
			value["value"] = filePathValue->string();
		else if (const auto colorValue = std::get_if<sf::Color>(&propertyValue))
			value["value"] = sf::utility::serializeColor(*colorValue);
		else if (const auto intValue = std::get_if<int>(&propertyValue))
			value["value"] = *intValue;
		else if (const auto floatValue = std::get_if<float>(&propertyValue))
			value["value"] = *floatValue;
		else if (const auto boolValue = std::get_if<bool>(&propertyValue))
			value["value"] = *boolValue;
		
		properties.append(value);
	}
}

void MapProperties::clearProperties()
{
	m_properties.clear();
}
