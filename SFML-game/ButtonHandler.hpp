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

	void setNextPosition(sf::Vector2i pos);
	sf::Vector2i getNextPosition() const;

	void setDefaultSize(sf::Vector2i size);
	sf::Vector2i getDefaultSize() const;

	void setDefaultTextSize(int size);
	int getDefaultTextSize() const;

	Button &addButton(const std::string &text, const std::function<void()> &func);

	auto &buttons() { return m_buttons; }
	const auto &buttons() const { return m_buttons; }
	
	/*template <typename Self>
	auto& operator[](this Self&& self, size_t index)
	{
		return self.m_buttons[index];
	}*/
private:
	enum class InputMode{ None, Mouse, Keys };
	InputMode updateInputMode();
	void handleKeyEvents(sf::Window &window);
	void handleMouseEvents(sf::Window &window); // TODO: implement, pollevent

private:

	// Variables for mouse/key-mode toggling and behaviour
	InputMode m_inputMode{ InputMode::None };
	sf::Vector2i m_lastMousePos;
	bool m_enterWasPressed{ true };
	sftools::Chronometer m_clock;
	sf::Time m_keyUpDownCooldown{ sf::milliseconds(100) };
	
	// Defaults for buttons
	sf::Vector2i m_defaultSize{ 250, 60 };
	int m_defaultTextSize = 40;
	sf::Vector2i m_posCurr;
	int m_spacing{ 10 };
	const sf::Font* m_pFont{ nullptr };
		
	// Buttons
	std::list<Button> m_buttons;
	decltype(m_buttons)::iterator m_it;
};

