#include "object/physic.hpp"

Physic::Physic() :
	vx(0.0f),
	vy(0.0f)
{}

void Physic::reset() { vx = 0; vy = 0; }

void Physic::set_velocity(float nvx, float nvy) { vx = nvx; vy = nvy; }
void Physic::set_velocity(const Vector& vector) { vx = vector.x; vy = vector.y; }
void Physic::set_velocity_x(float nvx) { vx = nvx; }
void Physic::set_velocity_y(float nvy) { vy = nvy; }

Vector Physic::get_velocity() const { return Vector(vx, vy); }
float Physic::get_velocity_x() const { return vx; }
float Physic::get_velocity_y() const { return vy; }

Vector Physic::get_movement(float dt_sec) {
	return Vector(vx, vy) * dt_sec;
}

void Physic::inverse_velocity_x() { vx = -vx; }
void Physic::inverse_velocity_y() { vy = -vy; }