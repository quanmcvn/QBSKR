#include "qbskr/waiting_screen.hpp"

#include "sprite/sprite_manager.hpp"
#include "sprite/sprite.hpp"
#include "qbskr/screen_fade.hpp"
#include "qbskr/screen_manager.hpp"
#include "video/compositor.hpp"
#include "video/drawing_context.hpp"

WaitingScreen::WaitingScreen(std::function<void()> callback) :
	m_callback(std::move(callback)),
	m_wait_timer(),
	m_sprite(SpriteManager::current()->create("images/menu/portal/portal-sprite.txt"))
{}

void WaitingScreen::setup()
{
	m_wait_timer.start(2, false);
}

void WaitingScreen::update(float /* dt_sec */, const Controller& /* controller */)
{
	if (m_wait_timer.check()) {
		ScreenManager::current()->pop_screen();
		// call the callback every time
		// although waiting screen is pop rightaway so call only one time
		// have no better design
		m_callback();
	}
}

void WaitingScreen::draw(Compositor& compositor)
{
	DrawingContext& drawing_context = compositor.make_context();
	drawing_context.get_canvas().draw_filled_rect(drawing_context.get_viewport(), Color(0.0f, 0.0f, 0.0f), LAYER_FOREGROUND);
	m_sprite->draw_scaled(drawing_context.get_canvas(), drawing_context.get_viewport(), LAYER_FOREGROUND + 1);
}