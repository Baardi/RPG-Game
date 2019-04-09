#pragma once
#include <map>
#include <functional>

template <class Base, class ...Args>
class ObjectFactory : public std::enable_if_t< std::has_virtual_destructor_v<Base> >
{
public:
	template <class Derived>
	bool registerType(const std::string &type)
	{
		static_assert(std::is_base_of_v<Base, Derived>, "type \"Derived\" is not derived from type \"Base\"");
		auto[it, inserted] = m_map.try_emplace(type, [](Args &&...args)
		{
			return std::make_unique<Derived>(std::forward<Args>(args)...);
		});

		return inserted;
	}

	template <class ...Args2>
	std::unique_ptr<Base> create(const std::string &type, Args2 &&...args) const
	{
		auto it = m_map.find(type);
		if (it != m_map.end())
			return it->second(std::forward<Args>(args)...);

		return std::make_unique<Base>(std::forward<Args>(args)...);
	}

private:
	std::map<std::string, std::function<std::unique_ptr<Base>(Args...)>> m_map;
};
