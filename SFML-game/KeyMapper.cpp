#include "stdafx.h"
#include "KeyMapper.h"

void KeyMapper::AddActionKey(sf::Keyboard::Key key, ControlKey isControlKey, std::function<void()> action)
{
	keys.emplace_back(key);
	actions.emplace_back(action);
	controlKeys.emplace_back(static_cast<bool>(isControlKey));
}

void KeyMapper::HandleKeyInput()
{
	bool controlKeyPressedNow = false;

	for (int i = keys.size() - 1 ; i >= 0; --i)
	{
		if (!sf::Keyboard::isKeyPressed(keys[i]))
			continue;

		if (controlKeys[i])
		{
			controlKeyPressedNow = true;
			if (controlKeyWasPressed)
				break;
		}

		actions[i]();
		break;
	}

	controlKeyWasPressed = controlKeyPressedNow;
}
