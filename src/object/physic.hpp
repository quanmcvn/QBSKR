#ifndef HEADER_QBSKR_OBJECT_PHYSIC_HPP
#define HEADER_QBSKR_OBJECT_PHYSIC_HPP

#include "math/vector.hpp"

/**
 * Physic engine :D
 * Movement is calculated time-wise now
*/
class Physic final {
public:
	Physic();

private:
	float vx, vy;

public:
	void set_velocity(float nvx, float nvy);
	void set_velocity(const Vector& vector);
	void set_velocity_x(float nvx);
	void set_velocity_y(float nvy);

	Vector get_velocity() const;
	float get_velocity_x() const;
	float get_velocity_y() const;

	Vector get_movement(float dt_sec);

	void inverse_velocity_x();
	void inverse_velocity_y();
};

#endif