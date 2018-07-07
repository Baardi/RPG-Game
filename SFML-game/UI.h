#pragma once
#include <SFML/Graphics.hpp>

class UI
{
public:
	UI();
	virtual ~UI(){}
	
	virtual void init();
	
	virtual bool frame() { return false; }
	virtual void pause(){}
	virtual void resume(){}
	virtual void toggle(){}

	void setDrawOrder();
	void drawAll();
	virtual void draw() {}
	
	virtual bool PollEvent(sf::Event::EventType eventType); // When overriding, remember to call parent
	void HandleWindowEvents();
	
	void SetParent(UI *parent) { this->parent = parent; } // To stack Ui's upon each other

	bool paused = false;

protected:
	sf::RenderWindow &window;
	sf::Event &event;
	sf::Font &font;

	UI *parent = nullptr;

private:
	std::vector<UI *> drawStack;
};
