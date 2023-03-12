#ifndef HEADER_QBSKR_VIDEO_COMPOSITOR_HPP
#define HEADER_QBSKR_VIDEO_COMPOSITOR_HPP

#include <vector>
#include <memory>

class DrawingContext;
class DrawingRequest;
class VideoSystem;

/**
 * How all of this (suppose) to work (pt.2) (pt.1 is in src/video_system.hpp)
 *   - Compositor holds DrawingContext(s), also holds the memory of all DrawingRequests
 *   - DrawingContext holds Canvas(es), also holds a transform stack (although didn't used)
 *   - Canvas passes DrawingRequests to Painter
 *   - Surface is NOT SDL_Surface, it's literally a surface
 *     it holds a TexturePtr, an area (region), flip
 *     it's also the component of Sprite (will be implemented soonTM)
*/

/**
 * This class holds memory of drawing requests, 
 * holds DrawingContext(s) which hold Canvas(es),
 * also composes the screen (which is the reason of its name)
*/
class Compositor final {
public:
	~Compositor();

private:
	Compositor(const Compositor&) = delete;
	Compositor& operator=(const Compositor&) = delete;

private:
	VideoSystem& m_video_system;
	// holds memory of drawing requests
	std::vector<std::unique_ptr<DrawingRequest>> m_request_holder;
	std::vector<std::unique_ptr<DrawingContext>> m_drawing_contexts;

public:
	Compositor(VideoSystem& video_system);

	DrawingContext& make_context();

	void render();
};

#endif