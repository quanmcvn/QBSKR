#include "object/moving_sprite.hpp"

#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"

MovingSprite::MovingSprite() :
	m_sprite_name(),
	m_sprite(),
	m_layer(),
	m_flip()
{}

MovingSprite::MovingSprite(const Vector& pos, const std::string& sprite_name, 
                           int layer, CollisionGroup collision_group) :
	m_sprite_name(sprite_name),
	m_sprite(SpriteManager::current()->create(m_sprite_name)),
	m_layer(layer),
	m_flip(NO_FLIP)
{
	m_collision_object.set_pos(pos);
	m_collision_object.m_bounding_box.set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
	set_group(collision_group);
}

MovingSprite::MovingSprite(const Vector& pos, const Sprite* sprite,
	                         int layer, CollisionGroup collision_group) :
	m_sprite_name(""),
	m_sprite(sprite->clone()),
	m_layer(layer),
	m_flip(NO_FLIP)
{
	m_collision_object.set_pos(pos);
	m_collision_object.m_bounding_box.set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
	set_group(collision_group);
}

void MovingSprite::update(float /* dt_sec */)
{}

void MovingSprite::draw(DrawingContext& drawing_context)
{
	m_sprite->draw(drawing_context.get_canvas(), get_pos(), m_layer, m_flip);
}

std::string MovingSprite::class_name() { return "moving-sprite"; }
std::string MovingSprite::get_class_name() const { return class_name(); }

int MovingSprite::get_layer() const { return m_layer; }

void MovingSprite::set_action(const std::string& action)
{
	m_sprite->set_action(action);
	m_collision_object.set_size(m_sprite->get_current_hitbox_width(), m_sprite->get_current_hitbox_height());
}

float MovingSprite::get_angle() const { return m_sprite->get_angle(); }
void MovingSprite::set_angle(float angle) { m_sprite->set_angle(angle); }

Flip MovingSprite::get_flip() const { return m_flip; }
void MovingSprite::set_flip(Flip flip) { m_flip = flip; }