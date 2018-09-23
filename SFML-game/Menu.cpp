#include "stdafx.h"
#include "State.h"
#include "Menu.h"

Menu::Menu()
{
	clock.resume();
}

Menu::~Menu()
{
}

void Menu::init()
{
	UI::init();
}

bool Menu::frame()
{
	HandleWindowEvents();

	if (clock.getElapsedTime().asMilliseconds() > 100)
	{
		tick();
		clock.reset(true);
	}

	if (!mouseControl)
	{
		if (!ControlKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			SelectEntry();
		}
		else if (!ControlKeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			State::Pop();
		}
		else if (ControlKeyPressed && !(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))))
		{
			ControlKeyPressed = false;
		}
	}
	
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

	menuItems.emplace_back(text, font, textSize);
	menuItems.back().setFillColor(index ? colorUnselect : colorSelect);
	menuItems.back().setPosition(x, y + spacing * index);

	actions.emplace_back(action);

	return index;
}

void Menu::SelectEntry() const
{
	if (menuIndex < actions.size())
		actions[menuIndex]();
}

void Menu::HandleKeyEvents()
{
	if (menuIndex >= menuItems.size())
	{
		menuIndex = 0;

		if (!menuItems.empty())
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

	for (int index = 0; index < menuItems.size(); index++)
	{
		auto &menuItem = menuItems[index];

		if (menuItem.getGlobalBounds().contains(pos.x, pos.y))
		{
			menuIndex = index;
			menuItems[menuIndex].setFillColor(colorSelect);
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
}

