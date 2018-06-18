#pragma once
#include "UI.h"
#include "MainMenu.h"

// List of all the States in the application. Setting a state will cause a transition to on of these
enum class State
{
	None,		// Push no ui
	Game,
	MainMenu,
	SelectionMenu
};

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
class Machine
{
public:
	// Sets a transition, to be done after current frame
	static void Set(Transition transition, State State = State::None)
	{
		Instance().transition = transition;
		Instance().state = State;
	}

	// Completes a transition, sets everything back to none
	static void Complete()
	{
		Instance().transition = Transition::None;
		Instance().state = State::None;
	}

	static Transition GetTransition()
	{
		return Instance().transition;
	}

	static State GetState()
	{
		return Instance().state;
	}
	
	static UI *GetUI()
	{
		return Instance().StateStack.back();
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

	static bool IsRunning()
	{
		return Instance().StateStack.size() > 0;
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

private:
	Machine() = default;
	Machine(Machine const&) = delete;
	void operator=(Machine const&) = delete;

	static Machine& Instance()
	{
		static Machine instance;
		return instance;
	}

	std::vector<UI*> StateStack;
	State state = State::MainMenu;
	Transition transition = Transition::None;
	Initializer *initializer = nullptr;
};

