#pragma once

template<class ...KeysAndFunc>
inline KeyInfo &KeyHandler::onKeyComboPressed(KeysAndFunc &&...keysAndFunc)
{
	auto[keys, func] = detail::extractKeysAndFunc(std::forward<KeysAndFunc>(keysAndFunc)...);
	return m_keys.emplace_back(std::move(keys), true, std::forward<std::function<void()>>(func));
}

template<class ...KeysAndFunc>
inline KeyInfo &KeyHandler::whileKeyComboPressed(KeysAndFunc &&...keysAndFunc)
{
	auto[keys, func] = detail::extractKeysAndFunc(std::forward<KeysAndFunc>(keysAndFunc)...);
	return m_keys.emplace_back(std::move(keys), false, std::forward<std::function<void()>>(func));
}

namespace detail {

	template <class... KeysAndFunc>
	std::pair<std::vector<sf::Keyboard::Key>, std::function<void()>> extractKeysAndFunc(KeysAndFunc &&...keysAndFunc)
	{
		std::vector<sf::Keyboard::Key> keys;
		keys.reserve(sizeof...(KeysAndFunc) - 1);
		return extractKeysAndFuncPartial(std::move(keys), std::forward<KeysAndFunc>(keysAndFunc)...);
	}

	template <class FirstKey, class... KeysAndFunc>
	std::pair<std::vector<sf::Keyboard::Key>, std::function<void()>> extractKeysAndFuncPartial(std::vector<sf::Keyboard::Key> keys, FirstKey firstKey, KeysAndFunc &&...keysAndFunc)
	{
		static_assert(std::is_same_v<sf::Keyboard::Key, FirstKey>, "All parameters but the last must be of type \"sf::Keyboard::Key\"");
		keys.emplace_back(firstKey);
		return extractKeysAndFuncPartial(std::move(keys), std::forward<KeysAndFunc>(keysAndFunc)...);
	}

	template <class Func>
	std::pair<std::vector<sf::Keyboard::Key>, std::function<void()>> extractKeysAndFuncPartial(std::vector<sf::Keyboard::Key> keys, Func &&func)
	{
		static_assert(std::is_invocable_v<Func>, "Last parameter must be invokable");
		return std::make_pair(std::move(keys), std::forward<Func>(func));
	}
}
