#ifndef HEADER_QBSKR_OBJECT_GAME_OBJECT_HPP
#define HEADER_QBSKR_OBJECT_GAME_OBJECT_HPP

#include "util/uid.hpp"

#include <string>

class DrawingContext;

/**
 *  Base class for every game object
*/
class GameObject {
	friend class GameObjectManager;

public:
	GameObject();
	virtual ~GameObject();

private:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

private:
	// unique id
	UID m_uid;

	// flag to remove the object at the end of frame
	bool m_schedule_for_remove;

public:
	UID get_uid() const;
	void clear_uid();

	virtual void update(float dt_sec) = 0;
	virtual void draw(DrawingContext& drawing_context) = 0;

	// schedule this object to be removed at the end of frame
	virtual void remove_me();

	bool is_valid() const;

	static std::string class_name();
	virtual std::string get_class_name() const;

	// if true then only one of this is allowed in a GameObjectManager
	virtual bool is_singleton() const;

private:
	void set_uid(const UID& uid);
};

#endif