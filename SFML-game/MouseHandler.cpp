#include "stdafx.h"
#include "MouseHandler.hpp"

void MouseButtonInfo::invoke()
{
	m_func();
}

bool MouseButtonInfo::isPressed() const
{
	for (auto &button : m_buttons)
	{
		if (!sf::Mouse::isButtonPressed(button))
			return false;
	}
	return true;
}

std::vector<sf::Mouse::Button> MouseButtonInfo::buttons() const
{
	return m_buttons;
}

bool MouseButtonInfo::invokeIfReady()
{
	if (!isPressed())
	{
		m_wasPressed = false;
		return false;
	}

	bool isReady = !(m_isModal && m_wasPressed);
	if (isReady)
		m_func();

	m_wasPressed = true;
	return isReady;
}

MouseButtonInfo& MouseHandler::addButton(MouseButtonInfo buttonInfo)
{
	return m_buttons.emplace_back(buttonInfo);
}

MouseButtonInfo& MouseHandler::onButtonPressed(sf::Mouse::Button button, const std::function<void()>& func)
{
	return m_buttons.emplace_back(button, true, func);
}

MouseButtonInfo& MouseHandler::whileButtonPressed(sf::Mouse::Button button, const std::function<void()>& func)
{
	return m_buttons.emplace_back(button, false, func);
}

MouseButtonInfo& MouseHandler::onButtonComboPressed(sf::Mouse::Button buttons, const std::function<void()>& func)
{
	return m_buttons.emplace_back(std::move(buttons), true, func);
}

MouseButtonInfo& MouseHandler::whileButtonComboPressed(sf::Mouse::Button buttons, const std::function<void()>& func)
{
	return m_buttons.emplace_back(std::move(buttons), false, func);
}

void MouseHandler::handleMouseInput()
{
	for (auto& button : m_buttons)
	{
		button.invokeIfReady();
	}
}
