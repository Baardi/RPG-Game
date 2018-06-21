#pragma once
#include <SFML/chronometer.h>
#include "UI.h"

class Menu : public UI
{
public:
	Menu();
	virtual ~Menu();

	void init() override;
	bool frame() override;
	void pause() override;
	void resume() override;
	bool PollEvent(sf::Event::EventType eventType) override;
	void tick();
	void draw();

protected:
	size_t AddMenuItem(std::string text);
	virtual void SelectEntry() {}

	std::vector<sf::Text> menuItems;
	size_t menuIndex = 0;

private:

	void HandleKeyEvents();
	void HandleMouseEvents();

	//"Personal" class variables
	sftools::Chronometer clock;

	const sf::Color colorSelect = sf::Color::Yellow;
	const sf::Color colorUnselect = sf::Color::White;

	bool ControlKeyPressed = true;
	bool mouseControl = true;
};
