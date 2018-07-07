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
	friend class UI;
public:
	// To be used from any Ui

	// Sets a transition, to be done after current frame
	static void Set(Transition transition, UI *state = nullptr)
	{
		Instance().transition = transition;
		Instance().queuedState = state;
	}
	
	static bool IsRunning()
	{
		return Instance().StateStack.size() > 0;
	}

	static bool IsInTransition()
	{
		return bool(Instance().transition);
	}

	static int Size()
	{
		return Instance().StateStack.size();
	}

	static Initializer *GetInitializer()
	{
		return Instance().initializer;
	}

	static void SetInitializer(Initializer *initializer)
	{
		// In case clearing is forgotten, don't create memory leaks
		if (Instance().initializer)
			delete Instance().initializer;

		Instance().initializer = initializer;
	}

	static void ClearInitializer()
	{
		delete Instance().initializer;
		Instance().initializer = nullptr;
	}

protected:
	// Methods for UI and App
	
	State() = default;
	State(State const&) = delete;
	void operator=(State const&) = delete;
	static State& Instance()
	{
		static State instance;
		return instance;
	}

	static void Setup(sf::RenderWindow &window, sf::Event &event, sf::Font &font)
	{
		Instance().window = &window;
		Instance().event = &event;
		Instance().font = &font;
	}

	static UI *GetUI()
	{
		return Instance().StateStack.back();
	}

	// Completes a queued transition
	static void PerformTransition()
	{
		if (Instance().transition == Transition::Pop)
			Instance().Pop();

		else if (Instance().transition == Transition::Reset)
			Instance().Reset();

		Instance().transition = Transition::None;

		if (Instance().queuedState)
			Instance().PushQueuedState();
	}

private:
	// Inner methods, used inside Instance

	void PushQueuedState()
	{
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

	sf::RenderWindow *window;
	sf::Event *event;
	sf::Font *font;
	
	Initializer *initializer = nullptr;
};
