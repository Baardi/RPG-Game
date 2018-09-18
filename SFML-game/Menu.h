#pragma once
#include "UI.h"
#include <functional>

class Menu : public UI
{
public:
	Menu();
	virtual ~Menu();

protected:
	// Overrides from UI
	void init() override;
	bool frame() override;
	bool PollEvent(sf::Event::EventType eventType) override;
	void draw() override;

	// Methods used by derived classes
	size_t AddMenuItem(const std::string &text, std::function<void()> action = [](){});
	
	// Data members changable by derived classes
	int x = 400, y = 400;
	int spacing = 50;
	int textSize = 40;
	sf::Color colorSelect = sf::Color::Yellow;
	sf::Color colorUnselect = sf::Color::White;

private:

	void tick();
	void SelectEntry() const;
	void HandleKeyEvents();
	void HandleMouseEvents();

	//"Personal" class variables
	sftools::Chronometer clock;
	
	std::vector<sf::Text> menuItems;
	std::vector<std::function<void()>> actions;
	size_t menuIndex = 0;


	bool ControlKeyPressed = true;
	bool mouseControl = false;
};
