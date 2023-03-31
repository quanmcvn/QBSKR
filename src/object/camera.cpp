#include "object/camera.hpp"

#include "object/player.hpp"
#include "qbskr/room.hpp"
#include "video/drawing_context.hpp"

Camera::Camera() :
	m_translation(0.0f, 0.0f),
	m_screen_size(),
	m_scale(1.0f)
{}

Camera::~Camera()
{}

Camera::Camera(const Camera& other) :
	m_translation(other.m_translation),
	m_screen_size(other.m_screen_size),
	m_scale(other.m_scale)
{}

void Camera::update(float /* dt_sec */)
{
	// simple move the camera to the first player
	Player& player = *Room::get().get_players()[0];
	Vector player_pos = player.get_pos();
	m_translation.x = player_pos.x - m_screen_size.width / 2;
	m_translation.y = player_pos.y - m_screen_size.height / 2;
}

void Camera::draw(DrawingContext& drawing_context)
{
	m_screen_size = Size(drawing_context.get_width(), drawing_context.get_height());
}

std::string Camera::class_name() { return "camera"; }
std::string Camera::get_class_name() const { return class_name(); }

bool Camera::is_singleton() const { return true; }

Vector Camera::get_translation() const { return m_translation; }

std::unique_ptr<Camera> Camera::clone() const
{
	auto camera = std::make_unique<Camera>(*this);
	return camera;
}