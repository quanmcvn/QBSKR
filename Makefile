### After some scraping online, I have C-c C-v -ed the Makefile that only compile what needed to compile
### It's not fully fool-proof because of no dependency check
### So I will delete definition code in *.hpp files :D

OBJ_FILES = src/main.o                                                  \
            src/badguy/badguy_set.o                                     \
            src/badguy/badguy.o                                         \
            src/badguy/generic_badguy.o                                 \
            src/collision/collision_object.o                            \
            src/collision/collision_system.o                            \
            src/collision/collision.o                                   \
            src/control/controller.o                                    \
            src/control/input_manager.o                                 \
            src/control/keyboard_config.o                               \
            src/control/keyboard_manager.o                              \
            src/control/mouse_button_config.o                           \
            src/control/mouse_button_manager.o                          \
            src/control/mouse_motion_manager.o                          \
            src/math/anchor_point.o                                     \
            src/math/interpolate.o                                      \
            src/math/random.o                                           \
            src/math/rect.o                                             \
            src/math/rectf.o                                            \
            src/math/size.o                                             \
            src/math/sizef.o                                            \
            src/math/util.o                                             \
            src/math/vector.o                                           \
            src/object/camera.o                                         \
            src/object/game_object_manager.o                            \
            src/object/game_object.o                                    \
            src/object/moving_object.o                                  \
            src/object/moving_sprite.o                                  \
            src/object/physic.o                                         \
            src/object/player.o                                         \
            src/object/tile_map.o                                       \
            src/object/tile_set_parser.o                                \
            src/object/tile_set.o                                       \
            src/object/tile.o                                           \
            src/qbskr/constants.o                                       \
            src/qbskr/gameconfig.o                                      \
            src/qbskr/globals.o                                         \
            src/qbskr/main.o                                            \
            src/qbskr/room.o                                            \
            src/sprite/sprite_data.o                                    \
            src/sprite/sprite_manager.o                                 \
            src/sprite/sprite.o                                         \
            src/util/crappy_reader_data.o                               \
            src/util/crappy_reader.o                                    \
            src/util/log.o                                              \
            src/util/timer.o                                            \
            src/util/uid_generator.o                                    \
            src/util/uid.o                                              \
            src/video/sdl/sdl_painter.o                                 \
            src/video/sdl/sdl_renderer.o                                \
            src/video/sdl/sdl_surface_creator.o                         \
            src/video/sdl/sdl_texture.o                                 \
            src/video/sdl/sdl_video_system.o                            \
            src/video/sdl/sdlbased_video_system.o                       \
            src/video/canvas.o                                          \
            src/video/color.o                                           \
            src/video/compositor.o                                      \
            src/video/drawing_context.o                                 \
            src/video/surface.o                                         \
            src/video/texture_manager.o                                 \
            src/video/texture.o                                         \
            src/video/video_system.o                                    \
            src/video/viewport.o                                        \
            src/weapon/shooting_weapon/projectile/generic_projectile.o  \
            src/weapon/shooting_weapon/projectile/projectile_set.o      \
            src/weapon/shooting_weapon/projectile/projectile.o          \
            src/weapon/shooting_weapon/generic_shooting_weapon.o        \
            src/weapon/shooting_weapon/shooting_weapon.o                \
            src/weapon/weapon_set.o                                     \
            src/weapon/weapon.o                                         \

#CC specifies which compiler we're using
CC = g++

SDL2_PATH = D:/Libraries/SDL2/x86_64-w64-mingw32
SDL2_IMAGE_PATH = D:/Libraries/SDL2_image/x86_64-w64-mingw32

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I$(SDL2_PATH)/include/SDL2          \
                -I$(SDL2_IMAGE_PATH)/include/SDL2    \
                -I./src                              \

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L$(SDL2_PATH)/lib                   \
                -L$(SDL2_IMAGE_PATH)/lib             \

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -std=c++17 -Wall -Wextra

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32           \
               -lSDL2main          \
               -lSDL2              \
               -lSDL2_image        \

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = QBSKR
GFLAGS = -g

OBJDIR := bin
OBJS := $(addprefix $(OBJDIR)/, $(OBJ_FILES))

$(OBJDIR)/%.o : %.cpp
	@mkdir -p $(@D)
	$(CC) -c $(GFLAGS) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $< -o $@

#This is the target that compiles our executable
### Move the executable to ./data to (partial) fix the "../data/"
.PHONY : all
all: $(OBJS)
	$(CC) $(GFLAGS) $(OBJS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ./data/$(OBJ_NAME)

$(OBJS): | $(OBJDIR)

.PHONY : clean
### idk why it can execute shell/bash (idk what it is even more)
### anyway...
clean:
	rm -rf ./bin/src/
	rm ./data/QBSKR.exe