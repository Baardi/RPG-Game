#include "stdafx.h"
#include "State.h"
#include "Menu.h"

Menu::Menu()
{
	m_clock.resume();

	m_keyMapper.addActionKey(sf::Keyboard::Return, ControlKey::Yes, std::bind(&Menu::selectEntry, this));
	m_keyMapper.addActionKey(sf::Keyboard::Escape, ControlKey::Yes, State::Pop);
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
	
	if (m_clock.getElapsedTime().asMilliseconds() > 100)
	{
		tick();
		m_clock.reset(true);
	}

	if (!m_mouseControl)
		m_keyMapper.handleKeyInput();
	
	return true;
}

bool Menu::pollEvent(sf::Event::EventType eventType)
{
	switch (eventType)
	{
	case sf::Event::MouseMoved:
		m_mouseControl = true;
		return true;

	case sf::Event::KeyPressed:
		m_mouseControl = false;
		return true;

	case sf::Event::MouseButtonPressed:
		if (m_mouseControl && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			selectEntry();

		return true;

	default:
		return UI::pollEvent(eventType);
	}
}

void Menu::tick()
{	
	m_mouseControl ? handleMouseEvents() : handleKeyEvents();
}

size_t Menu::addMenuItem(const std::string &text, const std::function<void()> &action)
{
	size_t index = menuItems.size();

	auto &itemInserted = menuItems.emplace_back(text, font, textSize);
	itemInserted.setFillColor(index ? colorUnselect : colorSelect);
	itemInserted.setPosition(x, y + spacing * index);

	m_actions.emplace_back(action);

	return index;
}

size_t Menu::addMenuItem(const std::string &text, const sf::Sprite &sprite, const std::function<void()> &action)
{
	size_t index = addMenuItem(text, action);
	addMenuSprite(sprite, index);
	return index;
}

void Menu::addMenuSprite(const sf::Sprite& sprite, size_t index)
{
	auto &addedSprite = m_menusprites.emplace_back(sprite); // Yes I want a copy
	addedSprite.setPosition(x + spriteSpacing, y + spacing * index);
}

std::pair<int, int> Menu::GetMenuCoords(size_t index)
{
	// Gets the coordinates of the menu item in a (x,y) pair
	int itemXPos = x;
	int itemYPos = y + spacing * index;
	return std::make_pair(itemXPos, itemYPos);
}

void Menu::selectEntry() const
{
	if (m_menuIndex < m_actions.size())
		m_actions[m_menuIndex]();
}

void Menu::handleKeyEvents()
{
	if (menuItems.empty())
		return;

	if (m_menuIndex >= menuItems.size())
	{
		m_menuIndex = 0;
		menuItems[m_menuIndex].setFillColor(colorSelect);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		menuItems[m_menuIndex].setFillColor(colorUnselect);
		m_menuIndex = (menuItems.size() + (m_menuIndex - 1)) % menuItems.size();
		menuItems[m_menuIndex].setFillColor(colorSelect);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		menuItems[m_menuIndex].setFillColor(colorUnselect);
		m_menuIndex = (m_menuIndex + 1) % menuItems.size();
		menuItems[m_menuIndex].setFillColor(colorSelect);
	}
}

void Menu::handleMouseEvents()
{
	auto pos = sf::Mouse::getPosition(window);
	m_menuIndex = -1;

	for (int index = menuItems.size() - 1; index >= 0; --index)
	{
		auto &menuItem = menuItems[index];

		if (menuItem.getGlobalBounds().contains(pos.x, pos.y))
		{
			m_menuIndex = index;
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

	for (auto &item : m_menusprites)
		window.draw(item);
}
