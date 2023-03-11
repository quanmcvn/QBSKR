#OBJS specifies which files to compile as part of the project
OBJS = src/main.cpp                            \
       src/control/controller.cpp              \
       src/control/input_manager.cpp           \
       src/control/keyboard_config.cpp         \
       src/control/keyboard_manager.cpp        \
       src/control/mouse_button_config.cpp     \
       src/control/mouse_button_manager.cpp    \
       src/control/mouse_motion_manager.cpp    \
       src/math/anchor_point.cpp               \
       src/math/rect.cpp                       \
       src/math/rectf.cpp                      \
       src/math/size.cpp                       \
       src/math/sizef.cpp                      \
       src/math/vector.cpp                     \
       src/qbskr/gameconfig.cpp                \
       src/qbskr/globals.cpp                   \
       src/qbskr/main.cpp                      \
       src/object/player.cpp                   \
       src/util/log.cpp                        \
       src/video/sdl/sdl_painter.cpp           \
       src/video/sdl/sdl_renderer.cpp          \
       src/video/sdl/sdl_surface_creator.cpp   \
       src/video/sdl/sdl_texture.cpp           \
       src/video/sdl/sdl_video_system.cpp      \
       src/video/sdl/sdlbased_video_system.cpp \
       src/video/canvas.cpp                    \
       src/video/compositor.cpp                \
       src/video/drawing_context.cpp           \
       src/video/surface.cpp                   \
       src/video/texture_manager.cpp           \
       src/video/texture.cpp                   \
       src/video/video_system.cpp              \
       src/video/viewport.cpp                  \

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -ID:/Libraries/SDL2/x86_64-w64-mingw32/include/SDL2          \
                -ID:/Libraries/SDL2_image/x86_64-w64-mingw32/include/SDL2    \
                -I./src                                                      \

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LD:/Libraries/SDL2/x86_64-w64-mingw32/lib                   \
                -LD:/Libraries/SDL2_image/x86_64-w64-mingw32/lib             \

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -std=c++17 -Wall -Wextra

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32           \
               -lSDL2main          \
               -lSDL2              \
               -lSDL2_image        \

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = QBSKR

#This is the target that compiles our executable
### Move the executable to ./data to (partial) fix the "../data/"
all : $(OBJS)
	$(CC) -g $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ./data/$(OBJ_NAME)