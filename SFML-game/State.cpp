#include "stdafx.h"
#include "State.h"
#include <iostream>

template <class T>
void State::Set(Transition transition)
{
	Instance().transition = transition;
	Instance().queuedState = new T;
}


void State::Set(Transition transition, UI* state)
{
	Instance().transition = transition;
	Instance().queuedState = state;
}

template <class T>
void State::SetChild()
{
	Instance().transition = Transition::Push;
	Instance().queuedState = new T;
	Instance().queuedState->SetParent(GetUI());
}

void State::SetChild(UI* state)
{
	Instance().transition = Transition::Push;
	Instance().queuedState = state;
	Instance().queuedState->SetParent(GetUI());
}

bool State::IsRunning()
{
	return Instance().StateStack.size() > 0;
}

bool State::IsInTransition()
{
	return bool(Instance().transition);
}

int State::Size()
{
	return Instance().StateStack.size();
}

Initializer* State::GetInitializer()
{
	return Instance().initializer;
}

void State::SetInitializer(Initializer* initializer)
{
	// In case clearing is forgotten, don't create memory leaks
	if (Instance().initializer)
		delete Instance().initializer;

	Instance().initializer = initializer;
}

void State::ClearInitializer()
{
	delete Instance().initializer;
	Instance().initializer = nullptr;
}

bool State::IsCurrent(UI* state)
{
	return GetUI() == state;
}

State& State::Instance()
{
	static State instance;
	return instance;
}

void State::Setup(sf::RenderWindow& window, sf::Event& event, sf::Font& font)
{
	Instance().window = &window;
	Instance().event = &event;
	Instance().font = &font;
}

UI* State::GetUI()
{
	return Instance().StateStack.back();
}

void State::PerformTransition()
{
	std::cout << std::to_string(Size()) + std::string(" -> ");

	if (IsRunning())
		GetUI()->pause();

	if (Instance().transition == Transition::Pop)
		Instance().Pop();

	else if (Instance().transition == Transition::Reset)
		Instance().Reset();

	Instance().transition = Transition::None;

	if (Instance().queuedState)
		Instance().PushQueuedState();

	if (IsRunning())
		GetUI()->setDrawOrder();

	if (IsRunning())
		GetUI()->resume();

	std::cout << std::to_string(Size()) << std::endl;
}

void State::PushQueuedState()
{
	queuedState->init();
	StateStack.push_back(queuedState);
	queuedState = nullptr;
}

void State::Pop()
{
	delete StateStack.back();
	StateStack.pop_back();
}

void State::Reset()
{
	for (auto state : StateStack)
		delete state;

	StateStack.clear();
}


