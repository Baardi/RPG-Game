#include "stdafx.h"
#include "Button.hpp"

Button::Button(const sf::Font &font) : m_pFont(&font)
{
	m_text.setFont(font);
	m_sprite.setTexture(m_texture);
	deselect();
}

void Button::setPosition(sf::Vector2f pos)
{	
	// Move textbox
	m_sprite.setPosition(pos);

	// Always centralize text afterwards	
	auto bounds = m_sprite.getGlobalBounds();
	centralizeText(static_cast<sf::IntRect>(bounds));
}

sf::Vector2f Button::getPosition() const
{
	return m_sprite.getPosition();
}

void Button::setSize(sf::Vector2f size)
{
	// Scale textbox
	auto rect = m_sprite.getTextureRect();
	rect.width = static_cast<int>(size.x);
	rect.height = static_cast<int>(size.y);
	m_sprite.setTextureRect(rect);

	// Always centralize text afterwards	
	auto bounds = m_sprite.getGlobalBounds();
	centralizeText(static_cast<sf::IntRect>(bounds));
}

sf::Vector2f Button::getSize() const
{
	auto rect = m_sprite.getGlobalBounds();
	return sf::Vector2f(rect.width, rect.height);
}

bool Button::contains(sf::Vector2f pos) const
{
	return getGlobalBounds().contains(pos);
}

sf::FloatRect Button::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

void Button::onClicked(const std::function<void()> &func)
{
	m_func = func;
}

void Button::invoke() const
{
	m_func();
}

void Button::setText(const std::string &text)
{
	m_text.setString(text);
}

std::string Button::getText() const
{
	return m_text.getString();
}

void Button::setTextSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}

unsigned int Button::getTextSize() const
{
	return m_text.getCharacterSize();
}

void Button::setFont(const sf::Font &font)
{
	m_pFont = &font;
}

const sf::Font &Button::getFont() const
{
	if (!m_pFont)
		throw std::runtime_error("Font is missing, not supposed to happen");

	return *m_pFont;
}

void Button::centralizeText(sf::IntRect bounds)
{
	const auto textHeight = getTextSize();

	const auto newX = bounds.left + 0.1f * static_cast<float>(bounds.width);
	const auto newY = bounds.top + static_cast<float>(bounds.height) / 2.0f - static_cast<float>(textHeight) / 2.0f;
	m_text.setPosition({ newX, newY });
}

void Button::setSelectionColour(sf::Color colour)
{
	m_colourSelect = colour;
	if (m_selected)
		m_sprite.setColor(colour);
}

sf::Color Button::getSelectionColour() const
{
	return m_colourSelect;
}

void Button::setDeselectionColour(sf::Color colour)
{
	m_colourDeselect = colour;
	if (!m_selected)
		m_sprite.setColor(colour);
}

sf::Color Button::getDeselectionColour() const
{
	return m_colourDeselect;
}

void Button::setTextSelectionColour(sf::Color colour)
{
	m_textColourSelect = colour;
	if (m_selected)
		m_text.setFillColor(colour);
}

sf::Color Button::getTextSelectionColour() const
{
	return m_textColourSelect;
}

void Button::setTextDeselectionColour(sf::Color colour)
{
	m_textColourDeselect = colour;
	if (!m_selected)
		m_text.setFillColor(colour);
}

sf::Color Button::getTextDeselectionColour() const
{
	return m_textColourDeselect;
}

void Button::select()
{
	m_selected = true;
	m_text.setFillColor(m_textColourSelect);
	m_sprite.setColor(m_colourSelect);
}

void Button::deselect()
{
	m_selected = false;
	m_text.setFillColor(m_textColourDeselect);
	m_sprite.setColor(m_colourDeselect);
}

bool Button::selected() const
{
	return m_selected;
}

void Button::draw(sf::RenderTarget &target) const
{
	target.draw(m_sprite);
	target.draw(m_text);
}

