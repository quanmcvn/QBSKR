#include "qbskr/screen_manager.hpp"

#include <assert.h>

#include "control/input_manager.hpp"
#include "qbskr/constants.hpp"
#include "qbskr/globals.hpp"
#include "video/compositor.hpp"

ScreenManager::Action::Action(Type type_, std::unique_ptr<Screen> screen_) :
	type(type_),
	screen(std::move(screen_))
{}

ScreenManager::ScreenManager(VideoSystem& video_system, InputManager& input_manager) :
	m_video_system(video_system),
	m_input_manager(input_manager),
	last_ticks(0),
	elapsed_ticks(0),
	ms_per_step(static_cast<Uint32>(1000.0f / LOGICAL_FPS)),
	seconds_per_step(static_cast<float>(ms_per_step) / 1000.0f)
{}

ScreenManager::~ScreenManager() {}

void ScreenManager::push_screen(std::unique_ptr<Screen> screen)
{
	m_actions.emplace_back(Action::PUSH_ACTION, std::move(screen));
}

void ScreenManager::pop_screen()
{
	m_actions.emplace_back(Action::POP_ACTION);
}

void ScreenManager::process_events()
{
	m_input_manager.update();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		m_input_manager.process_event(event);

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
}

void ScreenManager::draw(Compositor& compositor)
{
	assert(!m_screen_stack.empty());

	m_screen_stack.back()->draw(compositor);

	compositor.render();
}

void ScreenManager::quit()
{
	m_actions.emplace_back(Action::QUIT_ACTION);
}

void ScreenManager::handle_screen_switch()
{
	while (!m_actions.empty()) {
		auto actions = std::move(m_actions);

		for (auto& action : actions) {
			switch (action.type) {
				case Action::POP_ACTION:
					m_screen_stack.pop_back();
					break;

				case Action::PUSH_ACTION:
					assert(action.screen);
					m_screen_stack.push_back(std::move(action.screen));
					break;

				case Action::QUIT_ACTION:
					m_screen_stack.clear();
					break;
			}
		}
	}
}

void ScreenManager::loop()
{
	Uint64 ticks = SDL_GetTicks64();
	elapsed_ticks += static_cast<Uint32>(ticks - last_ticks);
	last_ticks = ticks;

	if (elapsed_ticks < ms_per_step) {
		SDL_Delay(ms_per_step - elapsed_ticks);
	}

	int steps = elapsed_ticks / ms_per_step;

	// cap max step per frame
	// 4 step per frame = 16 fps
	steps = std::min(steps, 4);

	for (int i = 0; i < steps; ++i) {
		float dtime = seconds_per_step;
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