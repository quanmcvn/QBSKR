#include "object/player.hpp"

Player::Player() :
	m_pos(0, 0),
	m_surface(),
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