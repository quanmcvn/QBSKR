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
	float ax, ay;

public:
	void reset();

	void set_velocity(float nvx, float nvy);
	void set_velocity(const Vector& vector);
	void set_velocity_x(float nvx);
	void set_velocity_y(float nvy);

	Vector get_velocity() const;
	float get_velocity_x() const;
	float get_velocity_y() const;

	void inverse_velocity_x();
	void inverse_velocity_y();

	void set_acceleration(float nax, float nay);
	void set_acceleration(const Vector& vector);
	void set_acceleration_x(float nax);
	void set_acceleration_y(float nay);

	Vector get_acceleration() const;
	float get_acceleration_x() const;
	float get_acceleration_y() const;

	Vector get_movement(float dt_sec);
};

#endif