#pragma once
#include <functional>

enum class ControlKey : bool 
{ 
	No = false,
	Yes = true
};

class KeyMapper
{
	// Maps a key into an action within an UI
	// Should not be used in menus
public:
	KeyMapper() = default;
	~KeyMapper() = default;
	void addActionKey(sf::Keyboard::Key key, ControlKey controlKey, const std::function<void()> &action);
	void handleKeyInput();

private:
	bool controlKeyWasPressed = true;

	std::vector<sf::Keyboard::Key> m_keys;
	std::vector<bool> m_controlKeys;
	std::vector<std::function<void()>> m_actions;
};

