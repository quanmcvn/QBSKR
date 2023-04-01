#ifndef HEADER_QBSKR_OBJECT_GAME_OBJECT_MANAGER_HPP
#define HEADER_QBSKR_OBJECT_GAME_OBJECT_MANAGER_HPP

#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>

#include "object/game_object.hpp"
#include "util/uid_generator.hpp"

class DrawingContext;
class TileMap;

class GameObjectManager {
public:
	GameObjectManager();
	virtual ~GameObjectManager();

private:
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

private:
	UIDGenerator m_uid_generator;
	std::vector<std::unique_ptr<GameObject>> m_game_objects;
	// container for newly create objects, will be added in flush_game_objects()
	std::vector<std::unique_ptr<GameObject>> m_game_objects_new;
	std::unordered_map<UID, GameObject*> m_objects_by_uid;
	std::unordered_map<std::type_index, std::vector<GameObject*>> m_objects_by_type_index;

	// fast access to solid tilemaps
	std::vector<TileMap*> m_solid_tilemaps;

public:
	// add an object to the queue to be added next frame (not actually add object)
	GameObject& add_object(std::unique_ptr<GameObject> object);
	void clear_objects();

	// template magic to contruct GameObject and call add_object() in the function instead outside
	template<typename T, typename... Args>
	T& add(Args&&... args)
	{
		std::unique_ptr<T> object = std::make_unique<T>(std::forward<Args>(args)...);
		T& object_ref = *object;
		add_object(std::move(object));
		return object_ref;
	}

	void update(float dt_sec);
	void draw(DrawingContext& drawing_context);

	const std::vector<std::unique_ptr<GameObject>>& get_objects() const;
	// use with care
	std::vector<std::unique_ptr<GameObject>>& get_objects_non_const();
	// do the actual work of adding object
	void flush_game_objects();

	template<class T>
	T* get_object_by_uid(const UID& uid) const
	{
		auto it = m_objects_by_uid.find(uid);
		if (it == m_objects_by_uid.end()) {
			return nullptr;
		} else {
			// return static_cast<T*>(it->second);
			auto ptr = dynamic_cast<T*>(it->second);
			assert(ptr != nullptr);
			return ptr;
		}
	}

	const std::vector<GameObject*>& get_objects_by_type_index(std::type_index type_idx) const;

	template<class T>
	T& get_singleton_by_type() const
	{
		const auto& range = get_objects_by_type_index(typeid(T));
		assert(range.begin() != range.end());
		assert((*range.begin())->is_singleton());
		return **range.begin();
	}

	// a hook that is called to decide "should i really add this?"
	virtual bool before_object_add(GameObject& object) = 0;
	// a hook that is called to remove object
	virtual void before_object_remove(GameObject& object) = 0;

	const std::vector<TileMap*>& get_solid_tilemaps() const;
	// use with care
	std::vector<TileMap*>& get_solid_tilemaps_non_const();

private:
	// to avoid being overriden
	void this_before_object_add(GameObject& object);
	// to avoid being overriden
	void this_before_object_remove(GameObject& object);
};

#endif