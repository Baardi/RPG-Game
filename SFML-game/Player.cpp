#include "stdafx.h"
#include "Player.hpp"
#include "Map.hpp"
#include "InventoryUI.hpp"
#include "DialogInterface.hpp"
#include "TileLayer.hpp"
#include "Game.hpp"
#include "App/StateMachine.hpp"
#include "App/ResourceManager.hpp"

Player::Player([[maybe_unused]]sftools::Chronometer &clock, sf::Vector2f pos) :
	m_textStats{ resources().font() },
	m_textFighting{ resources().font() },
	m_texture("data/Player Sprites/Engineer.png"),
	m_tilesize{ static_cast<int>(m_texture.getSize().x) / 4, static_cast<int>(m_texture.getSize().y / 4) },
	m_sprite{ m_texture, {{ 0, static_cast<int>(m_dir) * m_tilesize.y }, { m_tilesize.x, m_tilesize.y }} }
{
	// Map keys to directions for the player
	m_dirMap.emplace(Dir::Left,	 sf::Keyboard::Key::Left);
	m_dirMap.emplace(Dir::Right, sf::Keyboard::Key::Right);
	m_dirMap.emplace(Dir::Up,	 sf::Keyboard::Key::Up);
	m_dirMap.emplace(Dir::Down,	 sf::Keyboard::Key::Down);
	
	// Map keys for actions (such as open inventory)
	m_actionMap.emplace(Action::Inventory,	sf::Keyboard::Key::I);
	m_actionMap.emplace(Action::Talk,		sf::Keyboard::Key::T);
	
	m_mouseHandler.onButtonPressed(sf::Mouse::Button::Left, [&]() 
		{
			if (!m_fightTimer)
				m_fightTimer.emplace().reset(true);
		});

	m_textFighting.setPosition(sf::Vector2f{ 0.0, 0.0 });
	m_textFighting.setCharacterSize(10);
	m_textFighting.setString("Fighting");

	m_textStats.setPosition(sf::Vector2f{ 0.0, 25.0 });
	m_textFighting.setCharacterSize(10);

	// Put these as default stats temporarily
	m_stats.HP = 30;
	m_stats.Damage = 5;
		
	Player::setPosition(pos);
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget &target)
{
	target.draw(m_sprite);
#ifdef _DEBUG
	drawDebugOutline(target);
#endif // _DEBUG
}

void Player::drawToWindow(sf::RenderTarget &target)
{
	target.draw(m_textStats);
	
	if (m_fightTimer)
		target.draw(m_textFighting);
}

void Player::fight(Entity& other)
{
	if (m_fightTimer)
	{
		hit(other, m_stats.Damage);
		m_fightTimer = std::nullopt;
	}
}

sf::FloatRect Player::getLocalBounds() const
{
	return m_sprite.getLocalBounds();
}

sf::Transform Player::getTransform() const
{
	return m_sprite.getTransform();
}

void Player::setPosition(sf::Vector2f pos)
{
	m_sprite.setPosition(pos);
}

sf::Vector2f Player::getPosition() const
{
	return m_sprite.getPosition();
}

void Player::takeItem(std::unique_ptr<GameItem> item)
{
	m_inventory.takeItem(std::move(item));
}

void Player::handleKeyInput(appstate::Game &game, Map &map)
{
	if (m_fightTimer && m_fightTimer->getElapsedTime() > sf::milliseconds(300))
		m_fightTimer = std::nullopt;

	m_textStats.setString(m_stats.toString());
	m_mouseHandler.handleMouseInput();
	m_keyHandler.handleKeyInput();

	bool isMoving = false;

	for (auto [dir, key] : m_dirMap)
	{
		if (sf::Keyboard::isKeyPressed(key))
		{
			m_dir = dir;
			isMoving = true;
			break;
		}
	}
	
	if (sf::Keyboard::isKeyPressed(m_actionMap[Action::Inventory]))
		game.stateMachine().pushChild<appstate::InventoryPopup>(m_inventory); // Inventory popup
	if (sf::Keyboard::isKeyPressed(m_actionMap[Action::Talk]))
		game.stateMachine().pushChild<appstate::DialogInterface>(); // Get sprite-id, then start the dialog tree that matches that sprite id. Give necessary parameters

	if (isMoving)
	{
		const auto newPos = move(m_dir, getPosition());

		auto collisionLayer = map.getTileLayer("Collision");
		if ((!collisionLayer || !collisionLayer->containsTexture(newPos)))
		{
			setPosition(newPos);
			game.updateDrawRect();
		}

		m_counter = (m_counter + 1) % m_counterMax;
		m_sprite.setTextureRect({{ static_cast<int>(static_cast<float>(m_counter) / (static_cast<float>(m_counterMax) / 4.0)) * m_tilesize.x, static_cast<int>(m_dir) * m_tilesize.y }, { m_tilesize.x, m_tilesize.y }});
	}
	else
	{
		m_counter = m_counter/m_counterMax;
		m_sprite.setTextureRect({{ 0, int(m_dir) * m_tilesize.y }, { m_tilesize.x, m_tilesize.y }});
	}
}

sf::Vector2f Player::move(Dir dir, sf::Vector2f prevPos) const
{
	sf::Vector2f newPos = prevPos;

	switch (dir)
	{
	case Dir::Down:
		newPos.y += m_speed;
		break;

	case Dir::Up:
		newPos.y -= m_speed;
		break;

	case Dir::Left:
		newPos.x -= m_speed;
		break;

	case Dir::Right:
		newPos.x += m_speed;
		break;
	}

	return newPos;
}
