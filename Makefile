### After some scraping online, I have C-c C-v -ed the Makefile that only compile what needed to compile
### It's not fully fool-proof because of no dependency check
### So I will delete definition code in *.hpp files :D

### (for myself in the future): create ./bin/src/* to match every single src/* folder

OBJ_FILES = src/main.o                            \
            src/control/controller.o              \
            src/control/input_manager.o           \
            src/control/keyboard_config.o         \
            src/control/keyboard_manager.o        \
            src/control/mouse_button_config.o     \
            src/control/mouse_button_manager.o    \
            src/control/mouse_motion_manager.o    \
            src/math/anchor_point.o               \
            src/math/rect.o                       \
            src/math/rectf.o                      \
            src/math/size.o                       \
            src/math/sizef.o                      \
            src/math/vector.o                     \
            src/qbskr/gameconfig.o                \
            src/qbskr/globals.o                   \
            src/qbskr/main.o                      \
            src/object/player.o                   \
            src/util/log.o                        \
            src/video/sdl/sdl_painter.o           \
            src/video/sdl/sdl_renderer.o          \
            src/video/sdl/sdl_surface_creator.o   \
            src/video/sdl/sdl_texture.o           \
            src/video/sdl/sdl_video_system.o      \
            src/video/sdl/sdlbased_video_system.o \
            src/video/canvas.o                    \
            src/video/compositor.o                \
            src/video/drawing_context.o           \
            src/video/surface.o                   \
            src/video/texture_manager.o           \
            src/video/texture.o                   \
            src/video/video_system.o              \
            src/video/viewport.o                  \

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
COMPILER_FLAGS = -g -std=c++17 -Wall -Wextra

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32           \
               -lSDL2main          \
               -lSDL2              \
               -lSDL2_image        \

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = QBSKR

OBJDIR := bin
OBJS := $(addprefix $(OBJDIR)/, $(OBJ_FILES))

$(OBJDIR)/%.o : %.cpp
	$(CC) -c $(INCLUDE_PATHS) $< -o $@

#This is the target that compiles our executable
### Move the executable to ./data to (partial) fix the "../data/"
.PHONY : all
all: $(OBJS)
	$(CC) $(OBJS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ./data/$(OBJ_NAME)

$(OBJS): | $(OBJDIR)