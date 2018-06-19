#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"

class PlayerSelectMenu : public Menu
{
public:
	PlayerSelectMenu(sf::RenderWindow &window, sf::Event &event, sf::Font &font);
	~PlayerSelectMenu() = default;

private:
	size_t INDEX_1PLAYER = -1;
	size_t INDEX_2PLAYER = -1;
	size_t INDEX_3PLAYER = -1;
	size_t INDEX_4PLAYER = -1;
	size_t INDEX_BACK = -1;

	void SelectEntry() override;
};
