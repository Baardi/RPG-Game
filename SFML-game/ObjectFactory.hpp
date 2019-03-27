#pragma once
#include <map>
#include <functional>

 template <class Base>
class ObjectFactory
{
public:
	template <class Derived>
	bool registerType(const std::string &type)
	{
		auto[it, inserted] = m_map.try_emplace(type, []
		{
			return std::make_unique<Derived>();
		});

		return inserted;
	}

	std::unique_ptr<Base> create(const std::string &type)
	{
		auto it = m_map.find(type);
		if (it != m_map.end())
			return it->second();

		return std::make_unique<Base>();
	}

private:
	std::map<std::string, std::function<std::unique_ptr<Base>()>> m_map;
};

