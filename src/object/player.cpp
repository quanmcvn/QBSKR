#include "object/player.hpp"

#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"

Player::Player() :
	m_pos(0, 0),
	m_sprite(SpriteManager::current()->create("images/charactors/knight/knight_sprite.txt")),
	m_movement(0, 0)
{}

Player::~Player()
{}

void Player::set_movement(const Vector movement)
{
	m_movement = movement;
}

void Player::update()
{
	m_pos += m_movement;
	m_movement = Vector();
}