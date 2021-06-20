#pragma once
#include <variant>
#include <filesystem>
#include <optional>

class MapProperties
{
public:
	using PropertyType = std::variant<std::string, std::filesystem::path, sf::Color, int, float, bool>;
	
	MapProperties() = default;
	virtual ~MapProperties() = default;
	
	template <class T>
	std::optional<T> getProperty(const std::string &propertyName) const
	{
		auto it = m_properties.find(propertyName);
		if (it == m_properties.end())
			return {};

		auto property = std::get_if<T>(&it->second);
		if (!property)
		  return {};

		return *property;
	}

protected:
	void loadProperties(const Json::Value &properties);
	void saveProperties(Json::Value &properties) const;
	void clearProperties();

	std::map<std::string, PropertyType> m_properties;
};
