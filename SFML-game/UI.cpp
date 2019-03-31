#include "stdafx.h"
#include "UI.hpp"

void UI::init()
{
}

void UI::setDrawOrder()
{
	m_drawStack.clear();
	for (auto curr = this; curr; curr = curr->m_parent)
		m_drawStack.push_back(curr);
}

void UI::drawAll()
{
	m_window->clear(sf::Color::Black);

	// Reverse iteration, so the parent is drawn in backround
	for (auto it = m_drawStack.rbegin(); it != m_drawStack.rend(); ++it)
		(*it)->draw(*m_window);
	
	m_window->display();
}

void UI::pause()
{
	if (m_pausable)
		m_paused = true;
}

void UI::resume()
{
	if (m_pausable)
		m_paused = false;
}

void UI::toggle()
{
	if (m_pausable)
		m_paused ? resume() : pause();
}
