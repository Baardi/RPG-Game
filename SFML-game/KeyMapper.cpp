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
	for (auto &entry : actionMap)
	{
		if (sf::Keyboard::isKeyPressed(entry.first))
		{
			entry.second();
			break;
		}
	}
}

