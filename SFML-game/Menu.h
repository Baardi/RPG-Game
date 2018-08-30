#pragma once
#include <SFML/chronometer.h>
#include "UI.h"
#include <functional>

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
	size_t AddMenuItem(const std::string &text, std::function<void()> action);
	void SelectEntry() const;

	std::vector<sf::Text> menuItems;
	std::vector<std::function<void()>> actions;
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
