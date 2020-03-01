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
			m_propertyMap[propertyName] = propertyValue.asString();
		else if (propertyType == "file")
			m_propertyMap[propertyName] = std::filesystem::path{ propertyValue.asString() };
		else if (propertyType == "color")
			m_propertyMap[propertyName] = sf::utility::parseColor(propertyValue.asString());
		else if (propertyType == "int")
			m_propertyMap[propertyName] = propertyValue.asInt();
		else if (propertyType == "float")
			m_propertyMap[propertyName] = propertyValue.asFloat();
		else if (propertyType == "bool")
			m_propertyMap[propertyName] = propertyValue.asBool();
	}
}

void MapProperties::saveProperties(Json::Value &properties) const
{
	for (const auto &[propertyName, propertyValue] : m_propertyMap)
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
		
		if (propertyValue.index() == 0)
			value["value"] = std::get<0>(propertyValue);
		else if (propertyValue.index() == 1)
			value["value"] = std::get<1>(propertyValue).string();
		else if (propertyValue.index() == 2)
			value["value"] = sf::utility::parseColor(std::get<2>(propertyValue));
		else if (propertyValue.index() == 3)
			value["value"] = std::get<3>(propertyValue);
		else if (propertyValue.index() == 4)
			value["value"] = std::get<4>(propertyValue);
		else if (propertyValue.index() == 5)
			value["value"] = std::get<5>(propertyValue);
		
		properties.append(value);
	}
}
