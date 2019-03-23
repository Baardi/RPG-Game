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

	// Sets a transition, to be done after current frame
	static void Set(Transition transition, std::unique_ptr<UI> &&state)
	{
		Instance().transition = transition;
		Instance().queuedState = std::move(state);
	}

	// Add a new ui, and hide, but doesn't remove the previous ones
	template<class T, class... _Valty>
	static void Push(_Valty&&... _Val)
	{
		Set(Transition::Push, std::make_unique<T>(
			std::forward<_Valty>(_Val)...));
	}

	// Replace current ui with a new one
	template<class T, class... _Valty>
	static void Switch(_Valty&&... _Val)
	{
		Set(Transition::Switch, std::make_unique<T>(
			std::forward<_Valty>(_Val)...));
	}

	// Remove all ui's and add a new one
	template<class T, class... _Valty>
	static void Reset(_Valty&&... _Val)
	{
		Set(Transition::Reset, std::make_unique<T>(
			std::forward<_Valty>(_Val)...));
	}

	// Remove current ui
	static void Pop()
	{
		Set(Transition::Pop);
	}

	// Close the app
	static void Exit()
	{
		Set(Transition::Exit);
	}

	// Put a child to current ui
	template<class T, class... _Valty>
	static void PushChild(_Valty&&... _Val)
	{
		Push<T>(std::forward<_Valty>(_Val)...);
		Instance().queuedState->setParent(GetUI());
	}

	// Hides, but doesn't remove the current ui, and put a child to the previous one
	template<class T, class... _Valty>
	static void ChainChild(_Valty&&... _Val)
	{
		auto prevParent = GetUI() ? GetUI()->getParent() : nullptr;
		Push<T>(std::forward<_Valty>(_Val)...);
		Instance().queuedState->setParent(prevParent);
	}

	// Removes the current ui, and put a child ui to previous one
	template<class T, class... _Valty>
	static void SwitchChild(_Valty&&... _Val)
	{
		auto size = Instance().Size();
		auto parent = size >= 2 ? Instance().StateStack[size - 1] : nullptr;
		Switch<T>(std::forward<_Valty>(_Val)...);
		Instance().queuedState->setParent(parent);
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
		return static_cast<int>(Instance().StateStack.size());
	}
	
	template<class T, class... _Valty>
	static void SetInitializer(_Valty&&... _Val)
	{
		Instance().initializer = std::make_unique<T>(
			std::forward<_Valty>(_Val)...);
	}

	template<class T>
	static void SetInitializer(T&& initializer)
	{
		Instance().initializer = std::make_unique<T>(
			std::forward<T>(initializer));
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
		return Instance().font;
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

	static UI *GetUI()
	{
		return Instance().currentUi;
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
		{
			Instance().currentUi = Instance().StateStack.back().get();
			GetUI()->setDrawOrder();
			GetUI()->resume();
		}

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
	UI *currentUi = nullptr;
	Transition transition = Transition::None;
	
	sf::Font font;
	TextureMap textures;
	std::unique_ptr<Initializer> initializer;
};
