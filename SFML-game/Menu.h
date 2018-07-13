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
	void draw() override;

protected:
	size_t AddMenuItem(const std::string &text);
	virtual void SelectEntry() const {}

	std::vector<sf::Text> menuItems;
	size_t menuIndex = 0;

	int x = 400, y = 400;
	int spacing = 50;
	int textSize = 40;

private:

	void HandleKeyEvents();
	void HandleMouseEvents();

	//"Personal" class variables
	sftools::Chronometer clock;

	const sf::Color colorSelect = sf::Color::Yellow;
	const sf::Color colorUnselect = sf::Color::White;

	bool ControlKeyPressed = true;
	bool mouseControl = false;
};
