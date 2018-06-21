#pragma once
#include <SFML/Graphics.hpp>

class State;

class UI
{
public:
	UI() {}
	virtual ~UI(){}
	virtual void init();
	virtual bool frame() { return false; }
	virtual void pause(){}
	virtual void resume(){}
	virtual void toggle(){}
	
	virtual bool PollEvent(sf::Event::EventType eventType); // When overriding, remember to call parent
	virtual void HandleWindowEvents();
	void Setup(State *state, sf::RenderWindow *window, sf::Event *event, sf::Font *font);

	bool paused = false;

	sf::RenderWindow *window = nullptr;
	sf::Event *event = nullptr;
	sf::Font *font = nullptr;
	State *state = nullptr;
};
