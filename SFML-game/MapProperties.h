#pragma once
#include <any>

class MapProperties
{
public:
	MapProperties() = default;
	virtual ~MapProperties() {}
	
	template <class T>
	bool GetProperty(const std::string &propertyName, T &property) const
	{
		auto it = propertyMap.find(propertyName);
		if (it == propertyMap.end())
			return false;

		const T *castedValue = std::any_cast<T>(&it->second);
		if (!castedValue)
			return false;

		property = *castedValue;
		return true;
	}

	template <class T>
	T GetProperty(const std::string &propertyName) const
	{
		// Unsafe to use
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
