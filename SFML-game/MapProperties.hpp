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

protected:
	void loadProperties(const Json::Value &properties);
	void saveProperties(Json::Value &properties) const;

	std::map<std::string, std::any> m_propertyMap;
};
