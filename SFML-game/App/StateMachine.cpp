#include "stdafx.h"
#include "UiStateMachine.hpp"

using ui::StateMachine;

void StateMachine::performTransition()
{
	// Completes a queued transition
	std::cout << std::to_string(size()) + std::string(" -> ");

	if (isRunning())
		m_currentState->pause();

	if (m_transition == Transition::Pop)
		m_stateStack.pop_back();

	else if (m_transition == Transition::Reset)
		m_stateStack.clear();

	m_transition = Transition::None;

	if (m_queuedState)
	{
		m_queuedState->m_window = m_window;
		m_queuedState->m_stateMachine = this;
		m_queuedState->init();
		m_stateStack.emplace_back(std::move(m_queuedState));
	}

	if (isRunning())
	{
		m_currentState = m_stateStack.back().get();
		m_currentState->buildDrawStack();
		m_currentState->resume();
	}

	std::cout << std::to_string(size()) << std::endl;
}

void StateMachine::handleWindowEvents()
{
	sf::Event event;
	while (m_window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			exit();
			break;

		case sf::Event::LostFocus:
			if (m_currentState)
				m_currentState->pause();

			m_respondable = false;
			break;

		case sf::Event::GainedFocus:
			m_respondable = true;
			break;
		}

		if (m_respondable)
			m_currentState->pollEvent(event.type);
	}
}

void StateMachine::setWindow(sf::RenderWindow &window)
{
	m_window = &window;
	for (auto &ui : m_stateStack)
		ui->m_window = &window;
}
