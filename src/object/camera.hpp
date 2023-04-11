#ifndef HEADER_QBSKR_OBJECT_CAMERA_HPP
#define HEADER_QBSKR_OBJECT_CAMERA_HPP

#include <memory>

#include "object/game_object.hpp"
#include "math/size.hpp"
#include "math/vector.hpp"

/**
 *  Camare is stuck at player position
*/
class Camera final : public GameObject {
public:
	Camera();
	~Camera() override;

public:
	Camera(const Camera& other);

private:
	Camera& operator=(const Camera&) = delete;

private:
	Vector m_translation;
	Size m_screen_size;
	float m_scale;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
	static std::string class_name();
	virtual std::string get_class_name() const;
	virtual bool is_singleton() const override;

public:
	Vector get_translation() const;
	std::unique_ptr<Camera> clone() const;
};

#endif