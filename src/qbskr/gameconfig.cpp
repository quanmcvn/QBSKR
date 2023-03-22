#include "qbskr/gameconfig.hpp"

#include <sstream>

#include "util/crappy_reader.hpp"
#include "util/log.hpp"

Config::Config() :
	window_size(640, 480),
	magnification(1.0f),
	keyboard_config(),
	mouse_button_config()
{}

Config::Config(const std::string& filename) :
	Config()
{
	CrappyReader cr(filename);
	while (cr.parse("gameconfig")) {}
	CrappyReaderData* crd = cr.get_root()->get_child("gameconfig");

	if (!crd) {
		std::ostringstream msg;
		msg << "File '" << filename << "' is not gameconfig file";
		throw std::runtime_error(msg.str());
	}

	std::vector<int> windowsize;
	if (crd->get("window-size", windowsize)) {
		if (windowsize.size() != 2) {
			throw std::runtime_error("window-size should specify width and height");
		}
		window_size.width = windowsize[0];
		window_size.height = windowsize[1];
	} else {
		log_warning << "Cound't found window-size in config, now using default: "
		            << window_size.width << "x" << window_size.height << std::endl;
	}

	if (!crd->get("magnification", magnification)) {
		log_warning << "Cound't found magnification in config, now using default: "
		            << magnification << std::endl;
	}
}