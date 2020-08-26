#pragma once
#include <variant>
#include <filesystem>

class MapProperties
{
public:
	using PropertyType = std::variant<std::string, std::filesystem::path, sf::Color, int, float, bool>;
	
	MapProperties() = default;
	virtual ~MapProperties() = default;
	
	template <class T>
	bool getProperty(const std::string &propertyName, T &property) const
	{
		auto it = m_properties.find(propertyName);
		if (it == m_properties.end())
			return false;

		auto foundProperty = std::get_if<T>(&it->second);
		if (!foundProperty)
		  return false;

		property = *foundProperty;
		return true;
	}

protected:
	void loadProperties(const Json::Value &properties);
	void saveProperties(Json::Value &properties) const;
	void clearProperties();

	std::map<std::string, PropertyType> m_properties;
};
