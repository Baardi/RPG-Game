#include "stdafx.h"
#include "ButtonHandler.hpp"

void ButtonHandler::setSpacing(int spacing)
{
	m_spacing = spacing;
}

int ButtonHandler::getSpacing() const
{
	return m_spacing;
}

void ButtonHandler::setNextPosition(int x, int y)
{
	m_xCurr = x;
	m_yCurr = y;
}

std::pair<int, int> ButtonHandler::getNextPosition() const
{
	return std::make_pair(m_xCurr, m_yCurr);
}

void ButtonHandler::setDefaultSize(int width, int height)
{
	m_defaultWidth = width;
	m_defaultHeight = height;
}

std::pair<int, int> ButtonHandler::getDefaultSize() const
{
	return std::make_pair(m_defaultWidth, m_defaultHeight);
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
	auto &button = m_buttons.emplace_back(*m_pFont);
	if (m_buttons.size() == 1)
	{
		button.select();
		m_it = m_buttons.begin();
	}

	button.setSize(m_defaultWidth, m_defaultHeight);
	button.setTextSize(m_defaultTextSize);
	button.setPosition(m_xCurr, m_yCurr);
	button.setText(text);
	button.setActionHandler(func);

	auto bounds = button.getGlobalBounds();
	m_yCurr = bounds.top + bounds.height + m_spacing;

	return button;
}

bool ButtonHandler::toogleInputMode()
{
	if (!m_mouseControl.has_value())
	{
		m_lastMousePos = sf::Mouse::getPosition();
		m_mouseControl = false;
	}

	auto currMousePos = sf::Mouse::getPosition();
	if (m_lastMousePos != currMousePos)
	{
		m_lastMousePos = currMousePos;
		m_mouseControl = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		m_mouseControl = false;
	}

	return m_mouseControl.value();
}

void ButtonHandler::handleInput(sf::Window &window)
{
	toogleInputMode() ? handleMouseEvents(window) : handleKeyEvents(window);
}

void ButtonHandler::draw(sf::RenderTarget &target) const
{
	for (auto &button : m_buttons)
		button.draw(target);
}

void ButtonHandler::handleKeyEvents(sf::Window &window)
{
	if (m_buttons.empty())
		return;

	if (m_it._Ptr == nullptr)
	{
		m_it = m_buttons.begin();
		m_it->select();
	}

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
	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		if (!m_enterWasPressed)
			m_it->invoke();
	}
	else
	{
		m_enterWasPressed = false;
	}
}

void ButtonHandler::handleMouseEvents(sf::Window &window)
{
	m_it._Ptr = nullptr;
	auto pos = sf::Mouse::getPosition(window);
	for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
	{
		if (it->contains(pos.x, pos.y) && !m_it._Ptr)
		{
			m_it = it;
			it->select();
		}
		else
		{
			it->deselect();
		}
	}

	if (m_it._Ptr && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		m_it->invoke();
}
