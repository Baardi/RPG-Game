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

	// Completes a queued transition
	void CompleteTransition()
	{
		if (GetTransition() == Transition::Pop)
			Pop();

		else if (GetTransition() == Transition::Reset)
			Reset();

		transition = Transition::None;

		if (queuedState)
		{
			queuedState->Setup(this, this->window, this->event, this->font);
			queuedState->init();
			
			Push(queuedState);
			queuedState = nullptr;
		}
	}

	Transition GetTransition() const
	{
		return this->transition;
	}

	UI *GetUI()
	{
		return this->StateStack.back();
	}

	void Setup(sf::RenderWindow *window, sf::Event *event, sf::Font *font)
	{
		this->window = window;
		this->event = event;
		this->font = font;
	}

	Initializer *GetInitializer() const
	{
		return this->initializer;
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
		delete this->initializer;
		this->initializer = nullptr;
	}

	bool IsRunning() const
	{
		return this->StateStack.size() > 0;
	}

	int Size() const
	{
		return this->StateStack.size();
	}

private:
	
	void Push(UI* ui)
	{
		StateStack.push_back(ui);
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

	sf::RenderWindow *window = nullptr;
	sf::Event *event = nullptr;
	sf::Font *font = nullptr;

	std::vector<UI*> StateStack;
	UI *queuedState = nullptr;
	Transition transition = Transition::None;
	Initializer *initializer = nullptr;
};
