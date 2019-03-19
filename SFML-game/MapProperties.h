#pragma once
#include <any>

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

		const T *castedValue = std::any_cast<T>(&it->second);
		if (!castedValue)
			return false;
		
		*property = *castedValue;
		return true;
	}

	template <class T>
	T getProperty(const std::string &propertyName) const
	{
		// Unsafe to use
		return std::any_cast<T>(m_propertyMap.find(propertyName)->second);
	}

	bool containsProperty(const std::string &propertyName) const
	{
		return m_propertyMap.find(propertyName) != m_propertyMap.end();
	}

protected:
	void loadProperties(const Json::Value &object);
	std::map<std::string, std::any> m_propertyMap;
};
