#pragma once
#include <SFML/Graphics.hpp>

class UI
{
public:
	UI(sf::RenderWindow &window, sf::Event &event, sf::Font &font) : window(window), event(event), font(font) {}
	virtual ~UI(){}
	virtual bool frame() { return false; }
	virtual void draw() {}
	virtual void pause(){}
	virtual void resume(){}
	virtual void toggle(){}
	
	virtual bool PollEvent(sf::Event::EventType eventType); // When overriding, remember to call parent
	virtual void HandleWindowEvents();

	bool paused = false;

	sf::RenderWindow &window;
	sf::Event &event;
	sf::Font &font;
};


