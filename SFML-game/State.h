#pragma once
#include "UI.h"
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
	template <class T>
	static void Set(Transition transition)
	{
		Instance().transition = transition;
		Instance().queuedState = new T;
	}
	
	static void Set(Transition transition, UI *state = nullptr)
	{
		Instance().transition = Transition::Push;
		Instance().queuedState = state;
	}

	template <class T>
	static void Push()
	{
		Instance().transition = Transition::Push;
		Instance().queuedState = new T;
	}

	static void Push(UI *state)
	{
		Instance().transition = Transition::Push;
		Instance().queuedState = state;
	}

	template <class T>
	static void Switch()
	{
		Instance().transition = Transition::Switch;
		Instance().queuedState = new T;
	}

	static void Switch(UI *state)
	{
		Instance().transition = Transition::Switch;
		Instance().queuedState = state;
	}

	template <class T>
	static void Reset()
	{
		Instance().transition = Transition::Reset;
		Instance().queuedState = new T;
	}

	static void Reset(UI *state)
	{
		Instance().transition = Transition::Reset;
		Instance().queuedState = state;
	}

	template <class T>
	static void PushChild()
	{
		Instance().transition = Transition::Push;
		Instance().queuedState = new T;
		Instance().queuedState->SetParent(GetUI());
	}

	static void PushChild(UI *state)
	{
		Instance().transition = Transition::Push;
		Instance().queuedState = state;
		Instance().queuedState->SetParent(GetUI());
	}

	static void Pop()
	{
		Instance().transition = Transition::Pop;
	}

	static void Exit()
	{
		Instance().transition = Transition::Exit;
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

	static bool IsCurrent(UI *state)
	{
		return GetUI() == state;
	}
	
	static const sf::Font &Font()
	{
		return *Instance().font;
	}

	static std::map<std::string, sf::Texture*> &Textures()
	{
		return Instance().textures;
	}

protected:
	// Methods for UI and App

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
		std::cout << std::to_string(Size()) + std::string(" -> ");

		if (IsRunning())
			GetUI()->pause();

		if (Instance().transition == Transition::Pop)
			Instance().IPop();

		else if (Instance().transition == Transition::Reset)
			Instance().IReset();

		Instance().transition = Transition::None;

		if (Instance().queuedState)
			Instance().PushQueuedState();

		if (IsRunning())
			GetUI()->setDrawOrder();

		if (IsRunning())
			GetUI()->resume();

		std::cout << std::to_string(Size()) << std::endl;
	}

private:
	// Inner methods called by public and protected methods

	State() = default;
	~State()
	{
		if (initializer)
			delete initializer;

		for (auto &entry : textures)
			delete entry.second;
	}

	State(State const&) = delete;
	State(State const&&) = delete;
	void operator=(State const&) = delete;
	void operator=(State const&&) = delete;

	void PushQueuedState()
	{
		queuedState->init();
		StateStack.push_back(queuedState);
		queuedState = nullptr;
	}
	
	void IPop()
	{
		delete StateStack.back();
		StateStack.pop_back();
	}

	void IReset()
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
	
	std::map<std::string, sf::Texture*> textures;
	Initializer *initializer = nullptr;
};
