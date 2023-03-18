#include "qbskr/room.hpp"

#include "video/drawing_context.hpp"
#include "collision/collision_system.hpp"
#include "object/moving_object.hpp"

Room* Room::s_current = nullptr;

Room::Room() :
	m_collision_system(std::make_unique<CollisionSystem>(*this))
{}

Room::~Room()
{
	clear_objects();
}

Room& Room::get() { assert(s_current != nullptr); return *s_current; }
Room* Room::current() { return s_current; }

void Room::update(float dt_sec)
{
	GameObjectManager::update(dt_sec);
	m_collision_system->update();
	flush_game_objects();
}

void Room::draw(DrawingContext& drawing_context)
{
	drawing_context.push_transform();

	GameObjectManager::draw(drawing_context);
	m_collision_system->draw_debug(drawing_context);

	drawing_context.pop_transform();
}

bool Room::before_object_add(GameObject& object)
{
	if (auto moving_object = dynamic_cast<MovingObject*>(&object)) {
		m_collision_system->add(moving_object->get_collision_object());
	}

	return true;
}

void Room::before_object_remove(GameObject& object)
{
	if (auto moving_object = dynamic_cast<MovingObject*>(&object)) {
		m_collision_system->remove(moving_object->get_collision_object());
	}
}