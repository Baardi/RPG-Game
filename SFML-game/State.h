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
	virtual ~Initializer() = 0;
};
inline Initializer::~Initializer() {}

using TextureMap = std::map<std::string, sf::Texture>;

// Class for storing and handling different states in the application
class State
{
	friend class App;
	friend class UI;
public:
	// To be used from any Ui

	// Sets a transition, to be done after current frame

	static void Set(Transition transition)
	{
		Instance().transition = transition;
		Instance().queuedState.reset();
	}
	
	static void Set(Transition transition, std::unique_ptr<UI> &&state)
	{
		Instance().transition = transition;
		Instance().queuedState = std::move(state);
	}

	template <class T>
	static void Push()
	{
		Set(Transition::Push, std::make_unique<T>());
	}

	template <class T>
	static void PushChild()
	{
		Push<T>();
		Instance().queuedState->SetParent(GetUI());
	}

	template <class T>
	static void Switch()
	{
		Set(Transition::Switch, std::make_unique<T>());
	}

	template <class T>
	static void SwitchChild()
	{
		Switch<T>();
		Instance().queuedState->SetParent(GetUI());
	}

	template <class T>
	static void Reset()
	{
		Set(Transition::Reset, std::make_unique<T>());
	}

	template <class T>
	static void ResetChild()
	{
		Reset<T>();
		Instance().queuedState->SetParent(GetUI());
	}

	static void Pop()
	{
		Set(Transition::Pop);
	}

	static void Exit()
	{
		Set(Transition::Exit);
	}

	static bool IsRunning()
	{
		return Instance().StateStack.size() > 0;
	}

	static bool IsInTransition()
	{
		return static_cast<bool>(Instance().transition);
	}

	static int Size()
	{
		return Instance().StateStack.size();
	}

	
	template<class T, class... _Valty>
	static void SetInitializer(_Valty&&... _Val)
	{
		Instance().initializer = std::make_unique<T>(
			std::forward<_Valty>(_Val)...);
	}

	static void SetInitializer(std::unique_ptr<Initializer> &&initializer)
	{
		Instance().initializer = std::move(initializer);
	}

	template <class T>
	static T *GetInitializer()
	{
		return dynamic_cast<T *>(Instance().initializer.get());
	}

	static void ClearInitializer()
	{
		Instance().initializer.reset();
	}

	static bool IsCurrent(UI *state)
	{
		return GetUI() == state;
	}
	
	static const sf::Font &Font()
	{
		return *Instance().font;
	}
	static TextureMap &Textures()
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
		return Instance().StateStack.back().get();
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
	~State() = default;

	State(const State &) = delete;
	State(State &&) = delete;
	State &operator=(const State &) = delete;
	State &operator=(State &&) = delete;

	void PushQueuedState()
	{
		queuedState->init();
		StateStack.emplace_back(std::move(queuedState));
	}
	
	void IPop()
	{
		StateStack.pop_back();
	}

	void IReset()
	{
		StateStack.clear();
	}

	std::vector<std::unique_ptr<UI>> StateStack;
	std::unique_ptr<UI> queuedState;
	Transition transition = Transition::None;

	sf::RenderWindow *window;
	sf::Event *event;
	sf::Font *font;
	
	TextureMap textures;
	std::unique_ptr<Initializer> initializer;
};
