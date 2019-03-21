#include "stdafx.h"
#include "State.h"
#include "Menu.h"

Menu::Menu()
{
	m_clock.resume();

	m_buttonHandler.setFont(font);
	m_keyHandler.onKeyPressed(sf::Keyboard::Escape, State::Pop);
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
		m_buttonHandler.handleInput(window);
		m_clock.reset(true);
	}
	
	m_keyHandler.handleKeyInput();
	
	return true;
}

bool Menu::pollEvent(sf::Event::EventType eventType)
{
	switch (eventType)
	{
	default:
		return UI::pollEvent(eventType);
	}
}

Button &Menu::addMenuItem(const std::string &text, const std::function<void()> &action)
{
	size_t index = m_buttonHandler.size();
	
	auto &button = m_buttonHandler.addButton(text, action);
	button.setSelectionColour(sf::Color::Transparent);
	button.setDeselectionColour(sf::Color::Transparent);
	button.setTextSelectionColour(colorSelect);
	button.setTextDeselectionColour(colorUnselect);
	button.setTextSize(textSize);
	button.setPosition(x, y + spacing * index);
	
	return button;
}

Button &Menu::addMenuItem(const std::string &text, const sf::Sprite &sprite, const std::function<void()> &action)
{
	auto &button = addMenuItem(text, action);
	addMenuSprite(sprite, button);
	return button;
}

sf::Sprite &Menu::addMenuSprite(const sf::Sprite& sprite, const Button &button)
{
	auto &addedSprite = m_menusprites.emplace_back(sprite); // Yes I want a copy

	auto buttonPos = button.getPosition();
	addedSprite.setPosition(buttonPos.x + spriteSpacing, buttonPos.y);

	return addedSprite;
}

void Menu::draw()
{
	m_buttonHandler.draw(window);

	for (auto &item : m_menusprites)
		window.draw(item);
}
