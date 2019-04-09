#include "stdafx.h"
#include "App/State.hpp"

using appstate::State;

void State::setParent(State *parent)
{ 
	this->m_parent = parent; 
}

State *State::getParent() const 
{ 
	return m_parent; 
}

void State::buildDrawStack()
{
	m_drawStack.clear();
	for (auto curr = this; curr; curr = curr->m_parent)
		m_drawStack.push_back(curr);
}

void State::drawAll()
{
	m_window->clear(sf::Color::Black);

	// Reverse iteration, so the parent is drawn in backround
	for (auto it = m_drawStack.rbegin(); it != m_drawStack.rend(); ++it)
		(*it)->draw(*m_window);
	
	m_window->display();
}

void State::pause()
{
	if (m_pausable)
		m_paused = true;
}

void State::resume()
{
	if (m_pausable)
		m_paused = false;
}

void State::toggle()
{
	if (m_pausable)
		m_paused ? resume() : pause();
}
