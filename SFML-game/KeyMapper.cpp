#include "stdafx.h"
#include "KeyMapper.h"

void KeyMapper::addActionKey(sf::Keyboard::Key key, ControlKey isControlKey, std::function<void()> action)
{
	m_keys.emplace_back(key);
	m_actions.emplace_back(action);
	m_controlKeys.emplace_back(static_cast<bool>(isControlKey));
}

void KeyMapper::handleKeyInput()
{
	bool controlKeyPressedNow = false;

	for (int i = m_keys.size() - 1 ; i >= 0; --i)
	{
		if (!sf::Keyboard::isKeyPressed(m_keys[i]))
			continue;

		if (m_controlKeys[i])
		{
			controlKeyPressedNow = true;
			if (controlKeyWasPressed)
				break;
		}

		m_actions[i]();
		break;
	}

	controlKeyWasPressed = controlKeyPressedNow;
}
