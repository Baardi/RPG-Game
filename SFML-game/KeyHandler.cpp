#include "stdafx.h"
#include "KeyHandler.hpp"

void KeyInfo::invoke() 
{ 
	m_func(); 
}

bool KeyInfo::isPressed() const
{
	for (auto &key : m_keys)
	{
		if (!sf::Keyboard::isKeyPressed(key))
			return false;
	}
	return true;
}

std::vector<sf::Keyboard::Key> KeyInfo::keys() const
{
	return m_keys; 
}

bool KeyInfo::invokeIfReady()
{
	if (!isPressed())
	{
		m_wasPressed = false;
		return false;
	}
	
	bool isReady = !(m_isModal && m_wasPressed);
	m_wasPressed = true;

	if (isReady)
		m_func();

	return isReady;
}

KeyInfo &KeyHandler::addKey(KeyInfo keyInfo)
{
	return m_keys.emplace_back(keyInfo);
}

KeyInfo &KeyHandler::onKeyPressed(sf::Keyboard::Key key, const std::function<void()> &func)
{
	return m_keys.emplace_back(key, true, func);
}

KeyInfo &KeyHandler::whileKeyPressed(sf::Keyboard::Key key, const std::function<void()> &func)
{
	return m_keys.emplace_back(key, false, func);
}

KeyInfo &KeyHandler::onKeyComboPressed(std::vector<sf::Keyboard::Key> keys, const std::function<void()>& func)
{
	return m_keys.emplace_back(std::move(keys), true, func);
}

KeyInfo &KeyHandler::whileKeyComboPressed(std::vector<sf::Keyboard::Key> keys, const std::function<void()>& func)
{
	return m_keys.emplace_back(std::move(keys), false, func);
}

void KeyHandler::handleKeyInput()
{
	for (auto &key : m_keys)
	{
		key.invokeIfReady();
	}
}
