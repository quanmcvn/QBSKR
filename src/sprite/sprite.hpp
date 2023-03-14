#ifndef HEADER_QBSKR_SPRITE_SPRITE_HPP
#define HEADER_QBSKR_SPRITE_SPRITE_HPP

#include "sprite/sprite_data.hpp"
#include "video/canvas.hpp"
#include "video/drawing_context.hpp"

class Sprite final {
public:
	~Sprite();

private:
	// Sprite(const Sprite& other);
	Sprite& operator=(const Sprite&) = delete;

public:
	Sprite(SpriteData& data);

public:
	// update and draw sprite
	void draw(Canvas& canvas, const Vector& pos, int layer, Flip flip = NO_FLIP);

	// get current total frame
	int get_frames() const;

	// get current frame index
	int get_current_frame() const;

	float get_current_frame_progess() const;

private:
	void update();

	SpriteData& m_data;

	// between 0 and 1
	float m_frame_progress;
	// between 0 and get_frames() - 1
	int m_frame_index;
	
	float m_last_ticks;

	const SpriteData::Action* m_action;
};

#endif