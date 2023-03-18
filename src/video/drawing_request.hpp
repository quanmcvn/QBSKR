#ifndef HEADER_QBSKR_VIDEO_DRAWING_REQUEST_HPP
#define HEADER_QBSKR_VIDEO_DRAWING_REQUEST_HPP

#include "math/rectf.hpp"
#include "video/color.hpp"
#include "video/texture.hpp"
#include "video/flip.hpp"

enum RequestType {
	TEXTURE = 0,
	FILLEDRECT = 1
};


// cast from XXXRequest* to DrawingRequest* (and reverse) is possible
// (note the *, can't cast values without losing data (and potentially expensive), but cast pointers is possible)

struct DrawingRequest {
	DrawingRequest() = delete;
	// to maybe use dynamic_cast<>
	virtual ~DrawingRequest() {}
	
	RequestType type;

	int layer;
	Flip flip;
	float alpha;
	Rect viewport;

	DrawingRequest(RequestType type_) :
		type(type_),
		layer(),
		flip(),
		alpha(),
		viewport()
	{}
};

struct TextureRequest : public DrawingRequest {
private:
	TextureRequest(const TextureRequest&) = delete;
	TextureRequest& operator=(const TextureRequest&) = delete;

public:
	TextureRequest() :
		DrawingRequest(TEXTURE),
		texture(),
		srcrects(),
		dstrects(),
		angles(),
		color()
	{}

	const Texture* texture;
	// to batch draw 
	std::vector<Rectf> srcrects;
	// to batch draw 
	std::vector<Rectf> dstrects;
	std::vector<float> angles;
	Color color;
};

struct FilledRectRequest : public DrawingRequest
{
	Rectf rect;
	Color color;

	FilledRectRequest() :
		DrawingRequest(FILLEDRECT),
		rect(),
		color()
	{}
};


#endif