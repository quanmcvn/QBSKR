#include "object/credit_scroller.hpp"

#include <cmath>

#include "control/input_manager.hpp"
#include "object/info_box_line.hpp"
#include "qbskr/screen_fade.hpp"
#include "qbskr/screen_manager.hpp"
#include "util/crappy_reader_data.hpp"
#include "util/log.hpp"
#include "util/string_util.hpp"
#include "video/drawing_context.hpp"

namespace {
	const float DEFAULT_SPEED = 10;
}

CreditScroller::CreditScroller(const CrappyReaderData* crd) :
	m_controller(&InputManager::current()->get_controller()),
	m_lines(),
	m_scroll(),
	m_default_speed(DEFAULT_SPEED),
	m_finished(),
	m_fading()
{
	const CrappyReaderData* crd_content = crd->get_child("content");
	if (!crd_content) {
		throw std::runtime_error("File doesn't contain content");
	}
	parse_content(crd_content);
}

void CreditScroller::parse_content(const CrappyReaderData* crd_content)
{
	for (const auto& child : crd_content->m_childs) {
		if (child->m_data == "image") {
			std::string image_filename = StringUtil::join_strings(child->convert_child(), ' ');
			m_lines.emplace_back(std::make_unique<InfoBoxLine>('!', image_filename));
		} else if (child->m_data == "text") {
			std::string type;
			std::string text;

			if (!child->get("type", type)) {
				type = "normal";
			}

			if (!child->get_child("string")) {
				throw std::runtime_error("Text entry require string");
			} else {
				text = StringUtil::join_strings(child->get_child("string")->convert_child(), ' ');
			}

			if (type == "normal") {
				m_lines.emplace_back(std::make_unique<InfoBoxLine>('\t', text));
			} else if (type == "small") {
				m_lines.emplace_back(std::make_unique<InfoBoxLine>(' ', text));
			} else if (type == "heading") {
				m_lines.emplace_back(std::make_unique<InfoBoxLine>('-', text));
			} else {
				log_warning << "Unknown text type '" << type << "'" << std::endl;
				m_lines.emplace_back(std::make_unique<InfoBoxLine>('\t', text));
			}
		} else if (child->m_data == "blank") {
			// Empty line
			m_lines.emplace_back(std::make_unique<InfoBoxLine>('\t', ""));
		} else if (child->m_data == "person") {
			std::string name;
			std::string info;

			// this is so ugly
			// consequences of bad design
			if (child->get_child("name")) {
				name = StringUtil::join_strings(child->get_child("name")->convert_child(), ' ');
				m_lines.emplace_back(std::make_unique<InfoBoxLine>('\t', name));
			}
			if (child->get_child("info")) {
				info = StringUtil::join_strings(child->get_child("info")->convert_child(), ' ');
				m_lines.emplace_back(std::make_unique<InfoBoxLine>(' ', info));
			}
		} else {
			log_warning << "Unknown token '" << child->m_data << "'" << std::endl;
		}
	}
}

void CreditScroller::update(float dt_sec)
{
	float speed = m_default_speed;
	if (m_controller) {
		// allow changing speed with up and down keys
		if (m_controller->hold(Control::UP)) {
			speed = -m_default_speed * 5;
		} else if (m_controller->hold(Control::DOWN)) {
			speed = m_default_speed * 5;
		}

		if (m_controller->pressed(Control::ESCAPE)) {
			m_fading = true;
			ScreenManager::current()->pop_screen();
			return;
		}
	}

	m_scroll += speed * dt_sec;

	if (m_scroll < 0) m_scroll = 0;

	if (m_finished && !m_fading) {
		m_fading = true;
		ScreenManager::current()->pop_screen();
	}
}

void CreditScroller::draw(DrawingContext& drawing_context)
{
	drawing_context.push_transform();
	drawing_context.set_translation(Vector(0.0f, 0.0f));
	drawing_context.set_scale(1.0f);

	const float ctx_w = static_cast<float>(drawing_context.get_width());
	const float ctx_h = static_cast<float>(drawing_context.get_height());

	float y = std::floor(ctx_h - m_scroll);

	{ // draw text
		for (const auto& line : m_lines) {
			if (y + line->get_height() >= 0 && ctx_h - y >= 0) {
				line->draw(
					drawing_context.get_canvas(),
					Rectf(
						0, y,
						ctx_w, y
					),
					LAYER_GUI
				);
			}

			y += std::floor(line->get_height());
		}
	}

	drawing_context.pop_transform();

	// close when done
	if (y < 0) {
		m_finished = true;
		set_default_speed(0.0f);
	}
}

void CreditScroller::set_default_speed(float default_speed) { m_default_speed = default_speed; }