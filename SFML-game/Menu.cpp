#include "stdafx.h"
#include "State.h"
#include "Menu.h"

Menu::Menu()
{
	clock.resume();

	keyMapper.AddActionKey(sf::Keyboard::Return, ControlKey::Yes, std::bind(&Menu::SelectEntry, this));
	keyMapper.AddActionKey(sf::Keyboard::Escape, ControlKey::Yes, State::Pop);
}

Menu::~Menu()
{
}

void Menu::init()
{
	UI::init();
	pausable = false;
}

bool Menu::frame()
{
	if (!UI::frame())
		return false;
	
	if (clock.getElapsedTime().asMilliseconds() > 100)
	{
		tick();
		clock.reset(true);
	}

	if (!mouseControl)
		keyMapper.HandleKeyInput();
	
	return true;
}

bool Menu::PollEvent(sf::Event::EventType eventType)
{
	switch (eventType)
	{
	case sf::Event::MouseMoved:
		mouseControl = true;
		return true;

	case sf::Event::KeyPressed:
		mouseControl = false;
		return true;

	case sf::Event::MouseButtonPressed:
		if (mouseControl && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			SelectEntry();

		return true;

	default:
		return UI::PollEvent(eventType);
	}
}

void Menu::tick()
{	
	mouseControl ? HandleMouseEvents() : HandleKeyEvents();
}

size_t Menu::AddMenuItem(const std::string &text, std::function<void()> action)
{
	size_t index = menuItems.size();

	auto &itemInserted = menuItems.emplace_back(text, font, textSize);
	itemInserted.setFillColor(index ? colorUnselect : colorSelect);
	itemInserted.setPosition(x, y + spacing * index);

	actions.emplace_back(action);

	return index;
}

size_t Menu::AddMenuItem(const std::string &text, const sf::Sprite &sprite, std::function<void()> action)
{
	size_t index = AddMenuItem(text, action);
	AddMenuSprite(sprite, index);
	return index;
}

void Menu::AddMenuSprite(const sf::Sprite& sprite, size_t index)
{
	auto &addedSprite = menusprites.emplace_back(sprite); // Yes I want a copy
	addedSprite.setPosition(x + spriteSpacing, y + spacing * index);
}

std::pair<int, int> Menu::GetMenuCoords(size_t index)
{
	// Gets the coordinates of the menu item in a (x,y) pair
	int itemXPos = x;
	int itemYPos = y + spacing * index;
	return std::make_pair(itemXPos, itemYPos);
}

void Menu::SelectEntry() const
{
	if (menuIndex < actions.size())
		actions[menuIndex]();
}

void Menu::HandleKeyEvents()
{
	if (menuItems.empty())
		return;

	if (menuIndex >= menuItems.size())
	{
		menuIndex = 0;
		menuItems[menuIndex].setFillColor(colorSelect);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		menuItems[menuIndex].setFillColor(colorUnselect);
		menuIndex = (menuItems.size() + (menuIndex - 1)) % menuItems.size();
		menuItems[menuIndex].setFillColor(colorSelect);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		menuItems[menuIndex].setFillColor(colorUnselect);
		menuIndex = (menuIndex + 1) % menuItems.size();
		menuItems[menuIndex].setFillColor(colorSelect);
	}
}

void Menu::HandleMouseEvents()
{
	auto pos = sf::Mouse::getPosition(window);
	menuIndex = -1;

	for (int index = menuItems.size() - 1; index >= 0; --index)
	{
		auto &menuItem = menuItems[index];

		if (menuItem.getGlobalBounds().contains(pos.x, pos.y))
		{
			menuIndex = index;
			menuItem.setFillColor(colorSelect);
		}
		else
		{
			menuItem.setFillColor(colorUnselect);
		}
	}
}

void Menu::draw()
{
	for (auto &menuItem : menuItems)
		window.draw(menuItem);

	for (auto &item : menusprites)
		window.draw(item);
}
