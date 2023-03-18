#ifndef HEADER_QBSKR_COLLISION_COLLISION_GROUP_HPP
#define HEADER_QBSKR_COLLISION_COLLISION_GROUP_HPP

enum CollisionGroup {
	// not tested for collisions
	COLLISION_GROUP_DISABLED = 0,

	// use for ordinary moving objects
	COLLISION_GROUP_MOVING,

	// use for obstacles
	COLLISION_GROUP_STATIC
};

#endif