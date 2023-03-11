#include "video/video_system.hpp"

#include <assert.h>

#include "util/log.hpp"
#include "video/sdl/sdl_video_system.hpp"

std::unique_ptr<VideoSystem> VideoSystem::create(VideoSystem::VideoType video_system)
{
	switch (video_system) {
		case VIDEO_SDL:
			log_info << "new SDL renderer\n";
			return std::make_unique<SDLVideoSystem>();
			break;

		case VIDEO_DEBUG:
			log_info << "new Debug renderer\n";
			log_warning << "Debug renderer NYI (Not Yet Implemented)" << "\n";
			throw std::runtime_error("Debug renderer NYI");
			break;

		default:
			log_fatal << "invalid video system" << std::endl;
			assert(false);
			break;
	}
	// to get rid of annoying waring "control reaches end of non-void function"
	// idk why it isn't happy
	return std::make_unique<SDLVideoSystem>();
}

VideoSystem::VideoSystem()
{}

VideoSystem::~VideoSystem()
{}