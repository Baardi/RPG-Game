#include "stdafx.h"
#include "KeyMapper.h"


KeyMapper::KeyMapper()
{
}


KeyMapper::~KeyMapper()
{
}

void KeyMapper::AddActionKey(sf::Keyboard::Key key, ControlKey controlKey, std::function<void()> action)
{
	actionMap.try_emplace(key, action);

	if (bool(controlKey))
		controlKeys.emplace_back(key);
}

bool KeyMapper::isControlKeyPressed()
{
	for (auto key : controlKeys)
	{
		if (sf::Keyboard::isKeyPressed(key))
			return true;
	}
	return false;
}

void KeyMapper::HandleKeyInput()
{
	bool controlKeyPressedNow = false;

	for (auto &entry : actionMap)
	{
		auto key = entry.first;
		auto &action = entry.second;

		if (sf::Keyboard::isKeyPressed(key))
		{
			if (std::find(controlKeys.begin(), controlKeys.end(), key) != std::end(controlKeys))
			{
				controlKeyPressedNow = true;
				if (controlKeyWasPressed)
					break;
			}

			action();
			break;
		}
	}

	controlKeyWasPressed = controlKeyPressedNow;
}

