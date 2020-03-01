#pragma once
#include <variant>
#include <filesystem>

using PropertyType = std::variant<std::string, std::filesystem::path, sf::Color, int, float, bool>;

class MapProperties
{
public:

	MapProperties() = default;
	virtual ~MapProperties() {}
	
	template <class T>
	bool getProperty(const std::string &propertyName, T *property) const
	{
		auto it = m_propertyMap.find(propertyName);
		if (it == m_propertyMap.end())
			return false;

		try
		{
			*property = std::get<T>(it->second);
			return true;
		}
		catch (std::bad_variant_access &)
		{
			return false;
		}
	}

protected:
	void loadProperties(const Json::Value &properties);
	void saveProperties(Json::Value &properties) const;

	std::map<std::string, PropertyType> m_propertyMap;
};
