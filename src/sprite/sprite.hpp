#ifndef HEADER_QBSKR_SPRITE_SPRITE_HPP
#define HEADER_QBSKR_SPRITE_SPRITE_HPP

#include "sprite/sprite_data.hpp"
#include "sprite/sprite_ptr.hpp"
#include "video/canvas.hpp"
#include "video/color.hpp"
#include "video/drawing_context.hpp"

class Sprite final {
public:
	~Sprite();

public:
	Sprite(const Sprite& other);

public:
	Sprite(SpriteData& data);

private:
	Sprite& operator=(const Sprite&) = delete;

public:
	SpritePtr clone() const;

	// update and draw sprite
	void draw(Canvas& canvas, const Vector& pos, int layer, Flip flip = NO_FLIP);

	// set action (state)
	void set_action(const std::string& name);

	// get current total frame
	int get_frames() const;
	
	// get current frame index
	int get_current_frame() const;

	float get_current_frame_progess() const;

	float get_current_hitbox_width() const;
	float get_current_hitbox_height() const;

	Vector get_current_offset() const;

	float get_angle() const;
	void set_angle(float angle);

	float get_alpha() const;
	void set_alpha(float alpha);

	Color get_color() const;
	void set_color(const Color& color);

private:
	void update();

	SpriteData& m_data;

	// between 0 and 1
	float m_frame_progress;
	// between 0 and get_frames() - 1
	int m_frame_index;
	float m_last_ticks;
	float m_angle;
	float m_alpha;
	Color m_color;

	const SpriteData::Action* m_action;
};

#endif