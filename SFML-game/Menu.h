#pragma once
#include "UI.h"
#include <functional>
#include "KeyMapper.h"

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
	size_t AddMenuItem(const std::string &text, const std::function<void()> &action = []{});
	size_t AddMenuItem(const std::string &text, const sf::Sprite &sprite, const std::function<void()> &action = [] {});
	std::pair<int, int> GetMenuCoords(size_t index); // Gets the coordinates of the menu item in a (x,y) pair

	// Data members changable by derived classes
	int x = 400, y = 400;
	int spacing = 50, spriteSpacing = 220;
	int textSize = 40;
	sf::Color colorSelect = sf::Color::Yellow;
	sf::Color colorUnselect = sf::Color::White;

private:

	void AddMenuSprite(const sf::Sprite& sprite, size_t index);
	void tick();
	void SelectEntry() const;
	void HandleKeyEvents();
	void HandleMouseEvents();

	//"Personal" class variables
	sftools::Chronometer clock;

	size_t menuIndex = 0;
	std::vector<sf::Text> menuItems;
	std::vector<std::function<void()>> actions;
	std::vector<sf::Sprite> menusprites;

	KeyMapper keyMapper;
	bool ControlKeyPressed = true;
	bool mouseControl = false;
};
