#pragma once

class UI
{
	friend class StateHandler;
public:
	UI() = default;
	virtual ~UI() = default;
	
	virtual void init();
	virtual bool frame(sf::Window &window) { return true; }
	virtual void pollEvent(sf::Event::EventType eventType) {}
	
	virtual void pause();
	virtual void resume();
	void toggle();

	virtual void draw(sf::RenderTarget &target) {}
	void drawAll();
	void setDrawOrder();
	
	void setParent(UI *parent) { this->m_parent = parent; } // To stack Ui's upon each other
	UI *getParent() const { return m_parent; }
	
	sf::RenderWindow &window() { return *m_window; }
	StateHandler &stateHandler() { return *m_stateHandler; }
protected:
	bool m_paused = false;
	bool m_pausable = true;

private:
	sf::RenderWindow *m_window; 
	StateHandler *m_stateHandler;
	UI *m_parent = nullptr;
	std::vector<UI *> m_drawStack;
};
