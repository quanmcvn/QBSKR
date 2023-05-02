#include "object/chest.hpp"

#include "object/player.hpp"
#include "qbskr/room.hpp"
#include "video/drawing_context.hpp"
#include "weapon/weapon_set.hpp"
#include "weapon/weapon.hpp"

Chest::~Chest()
{}

Chest::Chest(const Vector& pos) :
	MovingSprite(pos, "images/objects/chest/chest-sprite.txt"),
	m_open(false)
{
	m_sprite->set_action("close");
}

void Chest::draw(DrawingContext& drawing_context)
{
	// do the default drawing
	// nothing fancy
	MovingSprite::draw(drawing_context);
}

HitResponse Chest::collision(GameObject& other, const CollisionHit& /* hit */)
{
	if (dynamic_cast<Player*>(&other)) {
		// automatically open if player touches it
		try_open();
	}

	return ABORT_MOVE;
}

void Chest::try_open()
{
	if (m_open) return;

	// hardcoding chest only spawn a weapon
	Room::get().add_object(WeaponSet::current()->get_weapon(3).clone(nullptr, get_pos()));
	m_sprite->set_action("open");
	m_open = true;
}