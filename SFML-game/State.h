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

// Singleton class storing and setting StateMachine-related information
class State
{
public:
	// Sets a transition, to be done after current frame
	static void Set(Transition transition, UI *state = nullptr)
	{
		Instance().transition = transition;
		Instance().queuedState = state;
	}

	// Completes a transition, 
	static void CompleteTransition()
	{
		if (GetTransition() == Transition::Pop)
			Pop();

		else if (GetTransition() == Transition::Reset)
			Reset();

		Instance().transition = Transition::None;

		if (Instance().queuedState)
		{
			Push(Instance().queuedState);
			Instance().queuedState = nullptr;
			
			GetUI()->Setup(Instance().window, Instance().event, Instance().font);
			GetUI()->init();
		}
	}

	static Transition GetTransition()
	{
		return Instance().transition;
	}

	static UI *GetUI()
	{
		return Instance().StateStack.back();
	}

	static void Setup(sf::RenderWindow *window, sf::Event *event, sf::Font *font)
	{
		Instance().window = window;
		Instance().event = event;
		Instance().font = font;
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

	static bool IsRunning()
	{
		return Instance().StateStack.size() > 0;
	}

	static int Size()
	{
		return Instance().StateStack.size();
	}

private:
	State() = default;
	State(State const&) = delete;
	void operator=(State const&) = delete;

	static State& Instance()
	{
		static State instance;
		return instance;
	}

	
	static void Push(UI* ui)
	{
		Instance().StateStack.push_back(ui);
	}
	
	static void Pop()
	{
		delete Instance().StateStack.back();
		Instance().StateStack.pop_back();
	}

	static void Reset()
	{
		for (auto state : Instance().StateStack)
			delete state;

		Instance().StateStack.clear();
	}

	sf::RenderWindow *window = nullptr;
	sf::Event *event = nullptr;
	sf::Font *font = nullptr;

	std::vector<UI*> StateStack;
	UI *queuedState = nullptr;
	Transition transition = Transition::None;
	Initializer *initializer = nullptr;
};
