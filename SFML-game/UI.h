#pragma once
#include <thread>

class UI
{
public:
	UI();
	virtual ~UI(){}
	
	virtual void init();
	
	virtual bool frame() { return true; }
	virtual bool PollEvent(sf::Event::EventType eventType); // When overriding, remember to call parent
	void HandleWindowEvents();

	// Possibly belongs to state
	bool isRespondable() const { return respondable; }

	virtual void pause();
	virtual void resume();
	void toggle();

	virtual void draw() {}
	void setDrawOrder();
	void drawAll();
	
	void SetParent(UI *parent) { this->parent = parent; } // To stack Ui's upon each other
	UI *GetParent() const { return parent; }

	bool paused = false;
	bool pausable = true;

protected:
	sf::RenderWindow &window;
	sf::Event &event;
	sf::Font font;

	// arghelper
private:
	void makeRespondable(bool respondable) { this->respondable = respondable; }

	bool respondable = true; // Maybe need a test to see if it's valid on ui creation
	UI *parent = nullptr;
	std::vector<UI *> drawStack;
};
