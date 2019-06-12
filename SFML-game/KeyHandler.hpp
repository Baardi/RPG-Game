#pragma once
#include <functional>

class KeyInfo
{
public:
	KeyInfo(sf::Keyboard::Key key, bool isModal, const std::function<void()> &func)
		: m_keys{ key }, m_isModal(isModal), m_func(func) {}

	KeyInfo(std::vector<sf::Keyboard::Key> keys, bool isModal, const std::function<void()> &func)
		: m_keys(keys), m_isModal(isModal), m_func(func) {}

	void invoke();
	bool isPressed() const;
	std::vector<sf::Keyboard::Key> keys() const;
	bool invokeIfReady();

private:
	bool m_isModal = false;
	bool m_wasPressed = true;
	std::vector<sf::Keyboard::Key> m_keys;
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
	
	template <class... KeysAndFunc>
	KeyInfo &onKeyComboPressed(KeysAndFunc &&... keysAndFunc);

	template <class... KeysAndFunc>
	KeyInfo &whileKeyComboPressed(KeysAndFunc &&... keysAndFunc);

	void handleKeyInput();

private:
	std::list<KeyInfo> m_keys;
};

#include "KeyHandler.inl"
