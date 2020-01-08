#pragma once
#include <functional>

class MouseButtonInfo
{
public:
	MouseButtonInfo(sf::Mouse::Button button, bool isModal, const std::function<void()>& func)
		: m_buttons{ button }, m_isModal(isModal), m_func(func) {}

	MouseButtonInfo(std::vector<sf::Mouse::Button> buttons, bool isModal, const std::function<void()>& func)
		: m_buttons(buttons), m_isModal(isModal), m_func(func) {}

	void invoke();
	bool isPressed() const;
	std::vector<sf::Mouse::Button> buttons() const;
	bool invokeIfReady();

private:
	bool m_isModal = false;
	bool m_wasPressed = true;
	std::vector<sf::Mouse::Button> m_buttons;
	std::function<void()> m_func;
};

class MouseHandler
{
	// Maps a key to an action within
public:
	MouseHandler() = default;
	~MouseHandler() = default;

	MouseButtonInfo& addButton(MouseButtonInfo buttonInfo);
	MouseButtonInfo& onButtonPressed(sf::Mouse::Button button, const std::function<void()>& func);
	MouseButtonInfo& whileButtonPressed(sf::Mouse::Button button, const std::function<void()>& func);
	MouseButtonInfo& onButtonComboPressed(sf::Mouse::Button buttons, const std::function<void()>& func);
	MouseButtonInfo& whileButtonComboPressed(sf::Mouse::Button buttons, const std::function<void()>& func);

	void handleMouseInput();

private:
	std::list<MouseButtonInfo> m_buttons;
};
