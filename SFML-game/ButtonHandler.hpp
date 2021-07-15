#pragma once
#include "Button.hpp"
#include "KeyHandler.hpp"
#include <optional>

class ButtonHandler
{
public:
	ButtonHandler();
	~ButtonHandler();

	// Main methods. call these every frame
	void handleInput(sf::Window &window);
	void draw(sf::RenderTarget &target) const;

	size_t size() const noexcept { return m_buttons.size(); }

	void setFont(const sf::Font &font) { m_pFont = &font; }
	const sf::Font &getFont() const { return *m_pFont; }

	void setSpacing(int spacing);
	int getSpacing() const;

	void setNextPosition(int x, int y);
	std::pair<int, int> getNextPosition() const;

	void setDefaultSize(int width, int height);
	std::pair<int, int> getDefaultSize() const;

	void setDefaultTextSize(int size);
	int getDefaultTextSize() const;

	Button &addButton(const std::string &text, const std::function<void()> &func);

	auto &buttons() { return m_buttons; }
	const auto &buttons() const { return m_buttons; }

	//auto &operator[](size_t index) { return m_buttons[index]; }
	//const auto &operator[](size_t index) const { return m_buttons[index]; }
private:
	enum class InputMode{ None, Mouse, Keys };
	InputMode updateInputMode();
	void handleKeyEvents(sf::Window &window);
	void handleMouseEvents(sf::Window &window); // TODO: implement, pollevent

private:

	// Variables for mouse/key-mode toggling and behaviour
	InputMode m_inputMode;
	sf::Vector2i m_lastMousePos;
	bool m_enterWasPressed = true;
	sftools::Chronometer m_clock;
	sf::Time m_keyUpDownCooldown = sf::milliseconds(100);
	
	// Defaults for buttons
	int m_defaultWidth = 250, m_defaultHeight = 60;
	int m_defaultTextSize = 40;
	int m_xCurr = 0, m_yCurr = 0;
	int m_spacing = 10;
	const sf::Font *m_pFont = nullptr;
		
	// Buttons
	std::list<Button> m_buttons;
	decltype(m_buttons)::iterator m_it;
};

