#pragma once
#include <thread>

class UI
{
public:
	UI() = default;
	virtual ~UI() = default;
	
	virtual void init();
	virtual bool frame(sf::Window &window) { return true; }
	virtual bool pollEvent(sf::Event::EventType eventType); // When overriding, remember to call parent

	void handleWindowsEvents(sf::Window &window);

	// Possibly belongs to state
	bool isRespondable() const { return m_respondable; }

	virtual void pause();
	virtual void resume();
	void toggle();

	virtual void draw(sf::RenderTarget &target) {}
	void drawAll(sf::RenderWindow &window);
	void setDrawOrder();
	
	void setParent(UI *parent) { this->m_pParent = parent; } // To stack Ui's upon each other
	UI *getParent() const { return m_pParent; }

protected:
	bool m_paused = false;
	bool m_pausable = true;

private:
	bool m_respondable = true; // Maybe need a test to see if it's valid on ui creation
	UI *m_pParent = nullptr;
	std::vector<UI *> m_drawStack;
};
