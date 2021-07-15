#pragma once

namespace appstate {

class State
{
	friend class StateMachine;
public:
	State() = default;
	virtual ~State() = default;
	
	virtual void init() {}
	virtual bool frame() { return true; }
	virtual void pollEvent([[maybe_unused]]sf::Event::EventType &eventType) {}
	
	virtual void pause();
	virtual void resume();
	void toggle();

	virtual void draw([[maybe_unused]]sf::RenderTarget &target) {}
	void drawAll();
	
	sf::RenderWindow &window() { return *m_window; }
	StateMachine &stateMachine() { return *m_stateMachine; }

protected:
	bool m_paused = false;
	bool m_pausable = true;

private:
	// To stack Ui's upon each other
	void buildDrawStack();
	void setParent(State *parent);
	State *getParent() const;

	sf::RenderWindow *m_window; 
	StateMachine *m_stateMachine;
	State *m_parent = nullptr;
	std::vector<State *> m_drawStack;
};

}