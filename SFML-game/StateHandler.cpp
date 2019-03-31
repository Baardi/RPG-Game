#include "stdafx.h"
#include "StateHandler.hpp"

// Completes a queued transition

void StateHandler::performTransition()
{
	std::cout << std::to_string(size()) + std::string(" -> ");

	if (isRunning())
		m_currentUi->pause();

	if (m_transition == Transition::Pop)
		m_stateStack.pop_back();

	else if (m_transition == Transition::Reset)
		m_stateStack.clear();

	m_transition = Transition::None;

	if (m_queuedState)
	{
		m_queuedState->m_window = m_window;
		m_queuedState->m_stateHandler = this;
		m_queuedState->init();
		m_stateStack.emplace_back(std::move(m_queuedState));
	}

	if (isRunning())
	{
		m_currentUi = m_stateStack.back().get();
		m_currentUi->setDrawOrder();
		m_currentUi->resume();
	}
	else
	{
		m_currentUi = nullptr;
	}

	std::cout << std::to_string(size()) << std::endl;
}

void StateHandler::handleWindowEvents()
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
			if (m_currentUi)
				m_currentUi->pause();

			m_respondable = false;
			break;

		case sf::Event::GainedFocus:
			m_respondable = true;
			break;
		}

		if (m_respondable)
			m_currentUi->pollEvent(event.type);
	}
}

void StateHandler::setWindow(sf::RenderWindow &window)
{
	m_window = &window;
	for (auto &ui : m_stateStack)
		ui->m_window = &window;
}
