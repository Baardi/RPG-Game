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
	virtual bool PollEvent(sf::Event::EventType eventType); // When overriding, remember to call parent
	void HandleWindowEvents();

	virtual void draw() {}
	void setDrawOrder();
	void drawAll();
	
	void SetParent(UI *parent) { this->parent = parent; } // To stack Ui's upon each other
	UI *GetParent() const { return parent; }

	bool paused = false;

protected:
	sf::RenderWindow &window;
	sf::Event &event;
	sf::Font font;

	// arghelper
private:
	UI *parent = nullptr;
	std::vector<UI *> drawStack;
};
