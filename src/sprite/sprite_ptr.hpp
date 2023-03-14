#ifndef HEADER_QBSKR_SPRITE_SPRITE_PTR_HPP
#define HEADER_QBSKR_SPRITE_SPRITE_PTR_HPP

#include <memory>

class Sprite;

typedef std::unique_ptr<Sprite> SpritePtr;

#endif
