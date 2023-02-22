#include "object/player.hpp"

Player::Player() : 
	m_pos(0, 0), m_movement(0, 0), m_texture_size(24, 42) {}

Player::~Player() {
	SDL_DestroyTexture(m_texture);
}

void Player::set_movement(const Vector movement) {
	m_movement = movement;
}

void Player::update() {
	m_pos += m_movement;
	m_movement = Vector();
}