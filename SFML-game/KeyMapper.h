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
	void AddActionKey(sf::Keyboard::Key key, ControlKey controlKey, std::function<void()> action);
	void HandleKeyInput();

private:
	bool controlKeyWasPressed = true;

	std::vector<sf::Keyboard::Key> keys;
	std::vector<bool> controlKeys;
	std::vector<std::function<void()>> actions;
};

