#include "qbskr/screen_manager.hpp"

#include <assert.h>

#include "control/input_manager.hpp"
#include "gui/menu_manager.hpp"
#include "gui/menu_set.hpp"
#include "gui/menu.hpp"
#include "qbskr/constants.hpp"
#include "qbskr/globals.hpp"
#include "qbskr/screen_fade.hpp"
#include "video/compositor.hpp"

ScreenManager::Action::Action(Type type_, std::unique_ptr<Screen> screen_) :
	type(type_),
	screen(std::move(screen_))
{}

ScreenManager::ScreenManager(VideoSystem& video_system, InputManager& input_manager) :
	m_video_system(video_system),
	m_input_manager(input_manager),
	m_menu_set(std::make_unique<MenuSet>()),
	m_menu_manager(std::make_unique<MenuManager>()),
	last_ticks(0),
	elapsed_ticks(0),
	ms_per_step(static_cast<Uint32>(1000.0f / LOGICAL_FPS)),
	seconds_per_step(static_cast<float>(ms_per_step) / 1000.0f),
	m_speed(1.0f),
	m_actions(),
	m_screen_fade(),
	m_screen_stack()
{}

ScreenManager::~ScreenManager() {}

float ScreenManager::get_speed() const { return m_speed; }
void ScreenManager::set_speed(float speed) { m_speed = speed;}

void ScreenManager::push_screen(std::unique_ptr<Screen> screen, std::unique_ptr<ScreenFade> fade)
{
	m_screen_fade = std::move(fade);
	m_actions.emplace_back(Action::PUSH_ACTION, std::move(screen));
}

void ScreenManager::pop_screen(std::unique_ptr<ScreenFade> fade)
{
	m_screen_fade = std::move(fade);
	m_actions.emplace_back(Action::POP_ACTION);
}

void ScreenManager::process_events()
{
	m_input_manager.update();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		m_input_manager.process_event(event);

		m_menu_manager->process_event(event);

		switch (event.type) {
			case SDL_QUIT:
				quit();
				break;
		}
	}
}

void ScreenManager::update_gamelogic(float dt_sec)
{
	Controller& controller = m_input_manager.get_controller();

	if (!m_screen_stack.empty()) {
		m_screen_stack.back()->update(dt_sec, controller);
	}

	if (m_screen_fade) {
		m_screen_fade->update(dt_sec);
	}

	m_menu_manager->process_input(controller);
}

void ScreenManager::draw(Compositor& compositor)
{
	assert(!m_screen_stack.empty());

	m_screen_stack.back()->draw(compositor);

	auto& drawing_context = compositor.make_context();
	m_menu_manager->draw(drawing_context);

	if (m_screen_fade) {
		m_screen_fade->draw(drawing_context);
	}

	compositor.render();
}

void ScreenManager::quit(std::unique_ptr<ScreenFade> fade)
{
	m_screen_fade = std::move(fade);
	m_actions.emplace_back(Action::QUIT_ACTION);
}

bool ScreenManager::has_pending_fade() const
{
	return m_screen_fade && !m_screen_fade->done();
}

void ScreenManager::handle_screen_switch()
{
	if (has_pending_fade()) {
		// wait till the fade is completed before switching screens
		return;
	}

	m_screen_fade.reset();
	while (!m_actions.empty()) {
		// needed for call leave()
		Screen* current_screen = m_screen_stack.empty() ? nullptr : m_screen_stack.back().get();
		auto actions = std::move(m_actions);

		for (auto& action : actions) {
			switch (action.type) {
				case Action::POP_ACTION:
					assert(!m_screen_stack.empty());
					if (current_screen == m_screen_stack.back().get()) {
						m_screen_stack.back()->leave();
						current_screen = nullptr;
					}
					m_screen_stack.pop_back();
					break;

				case Action::PUSH_ACTION:
					assert(action.screen);
					m_screen_stack.push_back(std::move(action.screen));
					break;

				case Action::QUIT_ACTION:
					m_screen_stack.clear();
					current_screen = nullptr;
					break;
			}
		}

		if (!m_screen_stack.empty()) {
			if (current_screen != m_screen_stack.back().get()) {
				if (current_screen) current_screen->leave();
				m_screen_stack.back()->setup();
				m_speed = 1.0f;
			}
		}
	}
}

void ScreenManager::loop()
{
	Uint64 ticks = SDL_GetTicks64();
	elapsed_ticks += static_cast<Uint32>(ticks - last_ticks);
	last_ticks = ticks;

	if (elapsed_ticks > ms_per_step * 8) {
		// when the game loads up or levels are switched the
		// elapsed_ticks grows extremely large, so we just ignore those
		// large time jumps
		elapsed_ticks = 0;
	}

	if (elapsed_ticks < ms_per_step) {
		SDL_Delay(ms_per_step - elapsed_ticks);
		return;
	}

	g_real_time = static_cast<float>(ticks) / 1000.0f;

	int steps = elapsed_ticks / ms_per_step;

	// cap max step per frame
	// 4 step per frame = 16 fps
	steps = std::min(steps, 4);

	for (int i = 0; i < steps; ++i) {
		float dtime = seconds_per_step * m_speed;
		g_game_time += dtime;
		process_events();
		update_gamelogic(dtime);
		elapsed_ticks -= ms_per_step;
	}

	if (steps > 0 && (!m_screen_stack.empty())) {
		Compositor compositor(m_video_system);
		draw(compositor);
	}

	handle_screen_switch();
}

void ScreenManager::run()
{
	handle_screen_switch();

	while (!m_screen_stack.empty()) {
		loop();
	}
}