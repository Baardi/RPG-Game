#include "stdafx.h"
#include "ButtonHandler.hpp"

ButtonHandler::ButtonHandler()
{
	m_clock.resume();
}

ButtonHandler::~ButtonHandler()
{
}

void ButtonHandler::setSpacing(int spacing)
{
	m_spacing = spacing;
}

int ButtonHandler::getSpacing() const
{
	return m_spacing;
}

void ButtonHandler::setNextPosition(sf::Vector2i pos)
{
	m_posCurr = pos;
}

sf::Vector2i ButtonHandler::getNextPosition() const
{
	return m_posCurr;
}

void ButtonHandler::setDefaultSize(sf::Vector2i size)
{
	m_defaultSize = size;
}

sf::Vector2i ButtonHandler::getDefaultSize() const
{
	return m_defaultSize;
}

void ButtonHandler::setDefaultTextSize(int size) 
{ 
	m_defaultTextSize = size; 
}

int ButtonHandler::getDefaultTextSize() const 
{ 
	return m_defaultTextSize; 
}

Button &ButtonHandler::addButton(const std::string &text, const std::function<void()> &func)
{
	if (!m_pFont)
		throw std::runtime_error("No current font");

	auto &button = m_buttons.emplace_back(*m_pFont);
	if (m_buttons.size() == 1)
	{
		button.select();
		m_it = m_buttons.begin();
	}

	button.setSize(static_cast<sf::Vector2f>(m_defaultSize));
	button.setTextSize(m_defaultTextSize);
	button.setPosition(static_cast<sf::Vector2f>(m_posCurr));
	button.setText(text);
	button.onClicked(func);

	auto bounds = button.getGlobalBounds();
	m_posCurr.y = static_cast<int>(bounds.top) + static_cast<int>(bounds.height) + m_spacing;

	return button;
}

ButtonHandler::InputMode ButtonHandler::updateInputMode()
{
	auto currMousePos = sf::Mouse::getPosition();

	if (m_inputMode == InputMode::None)
	{
		m_lastMousePos = currMousePos;
		m_inputMode = InputMode::Keys;
	}

	if (m_lastMousePos != currMousePos)
	{
		m_lastMousePos = currMousePos;
		m_inputMode = InputMode::Mouse;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		m_inputMode = InputMode::Keys;
	}

	return m_inputMode;
}

void ButtonHandler::handleInput(sf::Window &window)
{
	switch (updateInputMode())
	{
	case InputMode::Keys:
		handleKeyEvents(window);
		break;

	case InputMode::Mouse:
		handleMouseEvents(window);
		break;
	}
}

void ButtonHandler::draw(sf::RenderTarget &target) const
{
	for (auto &button : m_buttons)
		button.draw(target);
}

void ButtonHandler::handleKeyEvents([[maybe_unused]]sf::Window &window)
{
	if (m_buttons.empty())
		return;
	
	if (m_it == m_buttons.end())
	{
		m_it = m_buttons.begin();
		m_it->select();
	}

	if (m_clock.getElapsedTime() > m_keyUpDownCooldown)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			m_it->deselect();
			if (m_it == m_buttons.begin())
				m_it = --m_buttons.end();
			else
				m_it--;
			m_it->select();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			m_it->deselect();
			if (m_it == --m_buttons.end())
				m_it = m_buttons.begin();
			else
				m_it++;
			m_it->select();
		}

		m_clock -= m_keyUpDownCooldown;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		if (!m_enterWasPressed)
			m_it->invoke();
		m_enterWasPressed = true;
	}
	else if (m_enterWasPressed)
	{
		m_enterWasPressed = false;
	}
}

void ButtonHandler::handleMouseEvents(sf::Window &window)
{
	m_it = m_buttons.end();
	const auto pos = sf::Mouse::getPosition(window);
	for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
	{
		if (it->contains(static_cast<sf::Vector2f>(pos)) && m_it == m_buttons.end())
		{
			m_it = it;
			it->select();
		}
		else
		{
			it->deselect();
		}
	}

	if (m_it != m_buttons.end() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_it->invoke();
}
