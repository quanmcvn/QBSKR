#include "qbskr/credit_screen.hpp"

#include <sstream>

#include "object/credit_scroller.hpp"
#include "object/info_box_line.hpp"
#include "util/crappy_reader.hpp"
#include "video/compositor.hpp"

CreditScreen::~CreditScreen()
{}

CreditScreen::CreditScreen(const std::string& filename) :
	m_credit_scroller()
{
	try {
		CrappyReader cr(filename);
		while (cr.parse("credit")) {}
		const CrappyReaderData* crd_credit = cr.get_root()->get_child("credit");
		if (!crd_credit) {
			throw std::runtime_error("File isn't a credit file");
		}

		m_credit_scroller = std::make_unique<CreditScroller>(crd_credit);

		float default_speed;
		if (crd_credit->get("speed", default_speed)) {
			m_credit_scroller->set_default_speed(default_speed);
		}
	} catch (std::exception& e) {
		std::ostringstream msg;
		msg << "Couldn't load file '" << filename << "': " << e.what() << std::endl;
		throw std::runtime_error(msg.str());
	}
}

void CreditScreen::setup()
{
	// NYI
}

void CreditScreen::update(float dt_sec, const Controller& /* controller */)
{
	// credit scroller will handle everything
	m_credit_scroller->update(dt_sec);
}

void CreditScreen::draw(Compositor& compositor)
{
	auto& drawing_context = compositor.make_context();

	// credit scroller will draw (almost) everything 
	// (will add something in CreditScreen soonTm)
	m_credit_scroller->draw(drawing_context);
}
