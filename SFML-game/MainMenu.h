#pragma once
#include <SFML/Graphics.hpp>
#include "SnakeBodyElem.h"
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu(sf::RenderWindow &window, sf::Event &event, sf::Font &font);
	~MainMenu() = default;

private:
	size_t INDEX_NEWGAME = -1;
	size_t INDEX_RESUME = -1;
	size_t INDEX_EXIT = -1;

	void SelectEntry() override;
};
