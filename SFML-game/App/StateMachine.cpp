#include "stdafx.h"
#include "App/StateMachine.hpp"

using appstate::StateMachine;

void StateMachine::performTransition()
{
	// Completes a queued transition
	std::print("{} -> ", std::to_string(size()));

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

	std::println("{}", std::to_string(size()));
}

void StateMachine::handleWindowEvents()
{
	// TODO: Seems that SFML has a bug, so m_window->handleEvents doesn't work
	while (auto event = m_window->pollEvent())
	{
		event->visit([&](auto&& arg) -> void {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, sf::Event::Closed>)
			{
				exit();
			}
			else if constexpr (std::is_same_v<T, sf::Event::FocusLost>)
			{
				if (m_currentState)
					m_currentState->pause();

				m_respondable = false;
			}
			else if constexpr (std::is_same_v<T, sf::Event::FocusGained>)
			{
				m_respondable = true;
			}

			if (m_respondable)
				m_currentState->handleEvent(arg);
			});
	}
}

void StateMachine::setWindow(sf::RenderWindow &window)
{
	m_window = &window;
	for (auto &ui : m_stateStack)
		ui->m_window = &window;
}

bool StateMachine::runFrame()
{	
	// Todo cooldown for keypress when switching state

	if (inTransition())
	{
		sftools::Chronometer clock;
		clock.reset(true);
		performTransition();
		std::println("{}", std::to_string(clock.getElapsedTime().asSeconds()));
	}

	if (!isRunning())
		return false;

	handleWindowEvents();
	if (isRespondable())
		m_currentState->frame();

	m_currentState->drawAll();
	return true;
}
