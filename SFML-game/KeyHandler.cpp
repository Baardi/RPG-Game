#include "stdafx.h"
#include "KeyHandler.hpp"

bool KeyInfo::invokeIfReady()
{
	if (!sf::Keyboard::isKeyPressed(m_key))
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

void KeyHandler::handleKeyInput()
{
	for (auto &key : m_keys)
	{
		key.invokeIfReady();
	}
}
