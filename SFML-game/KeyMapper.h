#pragma once
#include <functional>

enum class ControlKey { No, Yes };

class KeyMapper
{
	// Maps a key into an action within an UI
	// Should not be used in menus
public:
	KeyMapper();
	~KeyMapper();
	void AddActionKey(sf::Keyboard::Key key, ControlKey controlKey, std::function<void()> action);
	void HandleKeyInput();
	bool isControlKeyPressed();

private:
	std::vector<sf::Keyboard::Key> controlKeys;
	std::map<sf::Keyboard::Key, std::function<void()>> actionMap;
};

