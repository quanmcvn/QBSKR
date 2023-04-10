#ifndef HEADER_QBSKR_QBSKR_SCREEN_MANAGER_HPP
#define HEADER_QBSKR_QBSKR_SCREEN_MANAGER_HPP

#include <SDL.h>

#include <memory>
#include <vector>

#include "qbskr/screen.hpp"
#include "util/currenton.hpp"

class MenuManager;
class MenuSet;
class InputManager;
class VideoSystem;

class ScreenManager final : public Currenton<ScreenManager> {
public:
	~ScreenManager() override;

private:
	VideoSystem& m_video_system;
	InputManager& m_input_manager;
	std::unique_ptr<MenuSet> m_menu_set;
	std::unique_ptr<MenuManager> m_menu_manager;

	Uint32 last_ticks;
	Uint32 elapsed_ticks;
	const Uint32 ms_per_step;
	const float seconds_per_step;

	float m_speed;

	struct Action {
		enum Type { PUSH_ACTION, POP_ACTION, QUIT_ACTION };
		Type type;
		std::unique_ptr<Screen> screen;

		Action(Type type_, std::unique_ptr<Screen> screen_ = {});
	};

	std::vector<Action> m_actions;
	std::vector<std::unique_ptr<Screen>> m_screen_stack;

public:
	ScreenManager(VideoSystem& video_system, InputManager& input_manager);

public:
	float get_speed() const;
	void set_speed(float speed);
	void push_screen(std::unique_ptr<Screen> screen);
	void pop_screen();

private:
	void process_events();
	void update_gamelogic(float dt_sec);
	void draw(Compositor& compositor);

public:
	void quit();

private:
	void handle_screen_switch();

public:
	void loop();
	void run();
};

#endif