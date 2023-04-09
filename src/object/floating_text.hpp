#ifndef HEADER_QBSKR_OBJECT_FLOATING_TEXT_HPP
#define HEADER_QBSKR_OBJECT_FLOATING_TEXT_HPP

#include "math/vector.hpp"
#include "object/game_object.hpp"
#include "util/timer.hpp"
#include "video/color.hpp"

class FloatingText final : public GameObject {
private:
	static Color text_color;

private:
	Vector position;
	std::string text;
	Color color;
	Timer timer;

public:
	FloatingText(const Vector& pos, const std::string& text_, const Color& color_ = text_color);
	FloatingText(const Vector& pos, int number, const Color& color_ = text_color);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;
};

#endif