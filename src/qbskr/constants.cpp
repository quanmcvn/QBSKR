#include "qbskr/constants.hpp"

// logically, fps should be a stack of frames per second (Minecraft ref?)
const float LOGICAL_FPS = 64.0f;

// since every images of block (so far) is only 16x16, changing this is not recommended
const float BLOCK_SIZE = 16.0f;

// a small value for use in collision
const float EPSILON = 0.005f;