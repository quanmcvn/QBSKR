#include "object/physic.hpp"

Physic::Physic() :
	vx(0.0f),
	vy(0.0f),
	ax(0.0f),
	ay(0.0f)
{}

void Physic::reset() { vx = 0; vy = 0; ax = 0; ay = 0; }

void Physic::set_velocity(float nvx, float nvy) { vx = nvx; vy = nvy; }
void Physic::set_velocity(const Vector& vector) { vx = vector.x; vy = vector.y; }
void Physic::set_velocity_x(float nvx) { vx = nvx; }
void Physic::set_velocity_y(float nvy) { vy = nvy; }

Vector Physic::get_velocity() const { return Vector(vx, vy); }
float Physic::get_velocity_x() const { return vx; }
float Physic::get_velocity_y() const { return vy; }

void Physic::inverse_velocity_x() { vx = -vx; }
void Physic::inverse_velocity_y() { vy = -vy; }

void Physic::set_acceleration(float nax, float nay) { ax = nax; ay = nay; }
void Physic::set_acceleration(const Vector& vector) { ax = vector.x; ay = vector.y; }

void Physic::set_acceleration_x(float nax) { ax = nax; }
void Physic::set_acceleration_y(float nay) { ay = nay; }

Vector Physic::get_acceleration() const { return Vector(ax, ay); }
float Physic::get_acceleration_x() const { return ax; }
float Physic::get_acceleration_y() const { return ay; }

Vector Physic::get_movement(float dt_sec) {
	vx += ax * dt_sec;
	vy += ay * dt_sec;
	return Vector(vx, vy) * dt_sec;
}