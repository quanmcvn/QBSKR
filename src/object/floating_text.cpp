#include "object/floating_text.hpp"

#include "qbskr/resources.hpp"
#include "video/drawing_context.hpp"

namespace {
	const float LINGERING_TIME = 1.0f;
	const float MOVE_UP_DISTANCE = 10.0f;
	const float FADING_TIME = 0.35f;
}

FloatingText::FloatingText(const Vector& pos, const std::string& text_, const Color& color_) :
	position(pos),
	text(text_),
	color(color_),
	timer()
{
	timer.start(LINGERING_TIME, false);
}

FloatingText::FloatingText(const Vector& pos, int number, const Color& color_) :
	FloatingText(pos, std::to_string(number), color_)
{}

void FloatingText::update(float dt_sec)
{
	position.y -= MOVE_UP_DISTANCE * dt_sec;

	if (timer.check()) remove_me();
}

void
FloatingText::draw(DrawingContext& drawing_context)
{
	// make an alpha animation when disappearing
	float alpha;
	if (timer.get_timeleft() < FADING_TIME) {
		alpha = timer.get_timeleft() * 1.0f / FADING_TIME;
	} else {
		alpha = 1.0f;
	}

	drawing_context.push_transform();

	drawing_context.set_alpha(alpha);
	drawing_context.get_canvas().draw_text(Resources::normal_font, text, position, ALIGN_CENTER, LAYER_HUD + 1, color);

	drawing_context.pop_transform();
}