#pragma once
#include <functional>

class KeyInfo
{
public:
	KeyInfo(sf::Keyboard::Key key, bool isModal, const std::function<void()> &func)
		: m_key(key), m_isModal(isModal), m_func(func) {}

	void invoke() { m_func(); }
	bool isPressed() const { return sf::Keyboard::isKeyPressed(m_key); }
	sf::Keyboard::Key key() const { return m_key; }

	bool invokeIfReady();

private:
	bool m_isModal = false;
	bool m_wasPressed = true;
	sf::Keyboard::Key m_key;
	std::function<void()> m_func;
};

class KeyHandler
{
	// Maps a key into an action within an UI
	// Should not be used in menus
public:
	KeyHandler() = default;
	~KeyHandler() = default;
	
	KeyInfo &addKey(KeyInfo keyInfo);
	KeyInfo &onKeyPressed(sf::Keyboard::Key key, const std::function<void()> &func);
	KeyInfo &whileKeyPressed(sf::Keyboard::Key key, const std::function<void()> &func);

	void handleKeyInput();

private:
	std::list<KeyInfo> m_keys;
};

