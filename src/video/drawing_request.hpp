#ifndef HEADER_QBSKR_VIDEO_DRAWING_REQUEST_HPP
#define HEADER_QBSKR_VIDEO_DRAWING_REQUEST_HPP

#include "math/rectf.hpp"
#include "video/texture.hpp"

enum RequestType {
	TEXTURE = 0,
};

struct DrawingRequest {
	DrawingRequest() = delete;
	virtual ~DrawingRequest() {}
	
	RequestType type;

	DrawingRequest(RequestType type_) :
		type(type_)
	{}
};

struct TextureRequest : DrawingRequest {
private:
	TextureRequest(const TextureRequest&) = delete;
	TextureRequest& operator=(const TextureRequest&) = delete;
public:
	TextureRequest() :
		DrawingRequest(TEXTURE),
		texture()
	{}

	const Texture* texture;
	// to batch draw 
	std::vector<Rectf> srcrects;
	// to batch draw 
	std::vector<Rectf> dstrects;
};

#endif