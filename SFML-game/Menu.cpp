#include "stdafx.h"
#include "StateHandler.hpp"
#include "ResourceHandler.hpp"
#include "Menu.hpp"

Menu::Menu()
{
	m_clock.resume();

	m_buttonHandler.setFont(resourceHandler().font());
	m_keyHandler.onKeyPressed(sf::Keyboard::Escape, [this] { stateHandler().popState(); });
}

Menu::~Menu()
{
}

void Menu::init()
{
	UI::init();
	m_pausable = false;
}

bool Menu::frame(sf::Window &window)
{
	if (!UI::frame(window))
		return false;
	
	m_buttonHandler.handleInput(window);	
	m_keyHandler.handleKeyInput();
	
	return true;
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

void Menu::draw(sf::RenderTarget &target)
{
	m_buttonHandler.draw(target);

	for (auto &item : m_menusprites)
		target.draw(item);
}
