#pragma once
#include "App/State.hpp"
#include <iostream>

namespace appstate {

// The different transition types
enum class Transition
{
	None = 0,	//             Do nothing. Always turns to None after a transition
	Push = 1,	// Push:       Adds a new state to the stack
	Pop = 2,	// Pop/Switch: Removes a state from the stack. Can push a new one
	Reset = 3,	// Exit/Reset: Removes all states. Can push a new one 
};

// Class for storing and handling different ui-states in the application
class StateMachine
{
public:
	StateMachine() = default;
	~StateMachine() = default;

	StateMachine(const StateMachine &) = delete;
	StateMachine(StateMachine &&) = delete;
	StateMachine &operator=(const StateMachine &) = delete;
	StateMachine &operator=(StateMachine &&) = delete;
	
	// Sets a transition, to be done after current frame
	void setState(Transition transition)
	{
		m_transition = transition;
		m_queuedState.reset();
	}

	// Sets a transition, to be done after current frame
	void setState(Transition transition, std::unique_ptr<State> &&state)
	{
		m_transition = transition;
		m_queuedState = std::move(state);
	}

	// Add a new ui, and hide, but doesn't remove the previous ones
	template<class T, class... _Valty>
	void pushState(_Valty&&... _Val)
	{
		setState(Transition::Push, std::make_unique<T>(
			std::forward<_Valty>(_Val)...));
	}

	// Replace current ui with a new one
	template<class T, class... _Valty>
	void switchState(_Valty&&... _Val)
	{
		setState(Transition::Pop, std::make_unique<T>(
			std::forward<_Valty>(_Val)...));
	}

	// Remove all ui's and add a new one
	template<class T, class... _Valty>
	void reset(_Valty&&... _Val)
	{
		setState(Transition::Reset, std::make_unique<T>(
			std::forward<_Valty>(_Val)...));
	}

	// Remove current ui
	void popState()
	{
		setState(Transition::Pop);
	}

	// Close the app
	void exit()
	{
		setState(Transition::Reset);
	}

	// Put a child to current ui
	template<class T, class... _Valty>
	void pushChild(_Valty&&... _Val)
	{
		pushState<T>(std::forward<_Valty>(_Val)...);
		m_queuedState->setParent(m_currentState);
	}

	// Hides, but doesn't remove the current ui, and put a child to the previous one
	template<class T, class... _Valty>
	void chainChild(_Valty&&... _Val)
	{
		auto prevParent = m_currentState ? m_currentState->getParent() : nullptr;
		pushState<T>(std::forward<_Valty>(_Val)...);
		m_queuedState->setParent(prevParent);
	}

	// Removes the current ui, and put a child ui to previous one
	template<class T, class... _Valty>
	void switchChild(_Valty&&... _Val)
	{
		auto parent = size() >= 2 ? m_stateStack[size() - 1] : nullptr;
		switchState<T>(std::forward<_Valty>(_Val)...);
		m_queuedState->setParent(parent);
	}

	bool isRunning() const
	{
		return m_stateStack.size() > 0;
	}

	bool inTransition() const
	{
		return m_transition != Transition::None;
	}

	size_t size() const
	{
		return m_stateStack.size();
	}

	bool isCurrent(State *state) const
	{
		return m_currentState == state;
	}

	const State *currentUiState() const
	{
		return m_currentState;
	}

	State *currentUiState()
	{
		return m_currentState;
	}

	bool isRespondable() const 
	{ 
		return m_respondable; 
	}

	// Should only be called by App
	void setWindow(sf::RenderWindow &window);
	void performTransition();
	void handleWindowEvents();

private:	
	bool m_respondable = true;
	sf::RenderWindow *m_window = nullptr;
	std::vector<std::unique_ptr<State>> m_stateStack;
	std::unique_ptr<State> m_queuedState;
	State *m_currentState = nullptr;
	Transition m_transition = Transition::None;
};

}
