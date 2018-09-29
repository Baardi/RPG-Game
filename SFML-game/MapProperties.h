#pragma once
#include <any>

class MapProperties
{
public:
	MapProperties() = default;
	virtual ~MapProperties() {}
	
	template <class T>
	T GetProperty(const std::string &propertyName) const
	{
		return std::any_cast<T>(propertyMap.find(propertyName)->second);
	}

	bool ContainsProperty(const std::string &propertyName) const
	{
		return propertyMap.find(propertyName) != propertyMap.end();
	}

protected:
	void LoadProperties(Json::Value &object);
	std::map<std::string, std::any> propertyMap;
};
