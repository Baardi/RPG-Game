#pragma once
#include "UI.hpp"
#include <iostream>

// The different transition types
enum class Transition
{
	None = 0,	// Do nothing. Always turns to None after a transition
	Push = 1,	// Adds a new state to the stack
	Switch = 2,	// Removes a state from the stack, and push a new one (theoretically the same as pop)
	Pop = 2,	// Remove a state from the stack, continuing executing the previous state in stack
	Reset = 3,	// Removes all states, then and push a new one (theoretically the same as Exit)
	Exit = 3	// Removes all states, which causes the program to exit
};

// Class for storing and handling different states in the application
class StateHandler
{
public:
	StateHandler() = default;
	~StateHandler() = default;

	StateHandler(const StateHandler &) = delete;
	StateHandler(StateHandler &&) = delete;
	StateHandler &operator=(const StateHandler &) = delete;
	StateHandler &operator=(StateHandler &&) = delete;
	
	// Sets a transition, to be done after current frame
	void setState(Transition transition)
	{
		m_transition = transition;
		m_queuedState.reset();
	}

	// Sets a transition, to be done after current frame
	void setState(Transition transition, std::unique_ptr<UI> &&state)
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
		setState(Transition::Switch, std::make_unique<T>(
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
		setState(Transition::Exit);
	}

	// Put a child to current ui
	template<class T, class... _Valty>
	void pushChild(_Valty&&... _Val)
	{
		pushState<T>(std::forward<_Valty>(_Val)...);
		m_queuedState->setParent(m_currentUi);
	}

	// Hides, but doesn't remove the current ui, and put a child to the previous one
	template<class T, class... _Valty>
	void chainChild(_Valty&&... _Val)
	{
		auto prevParent = m_currentUi ? m_currentUi->getParent() : nullptr;
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

	bool isCurrent(UI *state) const
	{
		return m_currentUi == state;
	}

	const UI *getCurrentUI() const
	{
		return m_currentUi;
	}

	UI *getCurrentUI()
	{
		return m_currentUi;
	}

	bool isRespondable() const 
	{ 
		return m_respondable; 
	}

	// Should only be called by App
	void performTransition();
	void handleWindowEvents();
	void setWindow(sf::RenderWindow &window);

private:
	bool m_respondable = true;
	sf::RenderWindow *m_window = nullptr;
	std::vector<std::unique_ptr<UI>> m_stateStack;
	std::unique_ptr<UI> m_queuedState;
	UI *m_currentUi = nullptr;
	Transition m_transition = Transition::None;
};
