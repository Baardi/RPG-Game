#pragma once
#include "UI.h"

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

//	Abstract class for initializing new states
class Initializer
{
public:
	Initializer() = default;
	virtual ~Initializer() = default;
};

// Class for storing and handling different states in the application
class State
{
	friend class App;
public:
	State() = default;
	State(State const&) = delete;
	void operator=(State const&) = delete;

	// Sets a transition, to be done after current frame
	void Set(Transition transition, UI *state = nullptr)
	{
		this->transition = transition;
		this->queuedState = state;
	}
	
	bool IsRunning() const
	{
		return StateStack.size() > 0;
	}

	int Size() const
	{
		return StateStack.size();
	}

	Initializer *GetInitializer() const
	{
		return initializer;
	}

	void SetInitializer(Initializer *initializer)
	{
		// In case clearing is forgotten, don't create memory leaks
		if (this->initializer)
			delete this->initializer;

		this->initializer = initializer;
	}

	void ClearInitializer()
	{
		delete initializer;
		initializer = nullptr;
	}

private:
	
	// Completes a queued transition
	void PerformTransition(sf::RenderWindow *window, sf::Event *event, sf::Font *font)
	{
		if (transition == Transition::Pop)
			Pop();

		else if (transition == Transition::Reset)
			Reset();

		transition = Transition::None;

		if (queuedState)
			PushQueuedState(window, event, font);
	}
	
	UI *GetUI()
	{
		return StateStack.back();
	}

	void PushQueuedState(sf::RenderWindow *window, sf::Event *event, sf::Font *font)
	{
		queuedState->Setup(this, window, event, font);
		queuedState->init();

		StateStack.push_back(queuedState);
		queuedState = nullptr;
	}
	
	void Pop()
	{
		delete StateStack.back();
		StateStack.pop_back();
	}

	void Reset()
	{
		for (auto state : StateStack)
			delete state;

		StateStack.clear();
	}

	std::vector<UI*> StateStack;
	UI *queuedState = nullptr;
	Transition transition = Transition::None;
	
	Initializer *initializer = nullptr;
};
