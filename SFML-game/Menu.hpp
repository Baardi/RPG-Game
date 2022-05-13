#pragma once
#include "App/State.hpp"
#include <functional>
#include "KeyHandler.hpp"
#include "ButtonHandler.hpp"

namespace appstate {

class Menu : public appstate::State
{
public:
	Menu();
	virtual ~Menu();

protected:
	// Overrides from UI
	void init() override;
	bool frame() override;
	void draw(sf::RenderTarget &target) override;

	// Methods used by derived classes
	Button &addMenuItem(const std::string &text, const std::function<void()> &action = []{});
	Button &addMenuItem(const std::string &text, const sf::Sprite &sprite, const std::function<void()> &action = [] {});

	// Data members changable by derived classes
	sf::Vector2i pos{ 400, 400 };
	int spacing{ 50 }, spriteSpacing{ 220 };
	int textSize{ 40 };
	sf::Color colorSelect = sf::Color::Yellow;
	sf::Color colorUnselect = sf::Color::White;

private:
	sf::Sprite &addMenuSprite(const sf::Sprite& sprite, const Button &button);

	//"Personal" class variables
	sftools::Chronometer m_clock;

	size_t m_menuIndex = 0;
	std::vector<sf::Text> menuItems;
	std::vector<std::function<void()>> m_actions;
	std::vector<sf::Sprite> m_menusprites;

	KeyHandler m_keyHandler;
	ButtonHandler m_buttonHandler;
};

}