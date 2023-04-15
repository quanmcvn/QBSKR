### After some scraping online, I have C-c C-v -ed the Makefile that only compile what needed to compile
### It's not fully fool-proof because of no dependency check
### So I will delete definition code in *.hpp files :D

OBJ_FILES = main.o                                                  \
            audio/sdl_mix_chunk.o                                   \
            audio/sdl_mix_music.o                                   \
            audio/sound_manager.o                                   \
            badguy/badguy_set.o                                     \
            badguy/badguy.o                                         \
            badguy/balbusour.o                                      \
            badguy/generic_badguy.o                                 \
            collision/collision_object.o                            \
            collision/collision_system.o                            \
            collision/collision.o                                   \
            control/controller.o                                    \
            control/input_manager.o                                 \
            control/keyboard_config.o                               \
            control/keyboard_manager.o                              \
            control/mouse_button_config.o                           \
            control/mouse_button_manager.o                          \
            control/mouse_motion_manager.o                          \
            gui/item_action.o                                       \
            gui/item_horizontal_line.o                              \
            gui/item_label.o                                        \
            gui/menu_item.o                                         \
            gui/menu_manager.o                                      \
            gui/menu_set.o                                          \
            gui/menu_transition.o                                   \
            gui/menu.o                                              \
            gui/mouse_cursor.o                                      \
            math/interpolate.o                                      \
            math/random.o                                           \
            math/rect.o                                             \
            math/rectf.o                                            \
            math/size.o                                             \
            math/sizef.o                                            \
            math/util.o                                             \
            math/vector.o                                           \
            object/camera.o                                         \
            object/floating_text.o                                  \
            object/game_object_manager.o                            \
            object/game_object.o                                    \
            object/moving_object.o                                  \
            object/moving_sprite.o                                  \
            object/physic.o                                         \
            object/player.o                                         \
            object/tile_map.o                                       \
            object/tile_set_parser.o                                \
            object/tile_set.o                                       \
            object/tile.o                                           \
            qbskr/menu/game_menu.o                                  \
            qbskr/menu/main_menu.o                                  \
            qbskr/color_scheme.o                                    \
            qbskr/constants.o                                       \
            qbskr/gameconfig.o                                      \
            qbskr/game_session.o                                    \
            qbskr/globals.o                                         \
            qbskr/level_data_set.o                                  \
            qbskr/level_data.o                                      \
            qbskr/level.o                                           \
            qbskr/main.o                                            \
            qbskr/resources.o                                       \
            qbskr/room_data_set.o                                   \
            qbskr/room_data.o                                       \
            qbskr/room.o                                            \
            qbskr/screen_fade.o                                     \
            qbskr/screen_manager.o                                  \
            qbskr/screen.o                                          \
            qbskr/title_screen.o                                    \
            sprite/sprite_data.o                                    \
            sprite/sprite_manager.o                                 \
            sprite/sprite.o                                         \
            util/crappy_reader_data.o                               \
            util/crappy_reader.o                                    \
            util/line_iterator.o                                    \
            util/log.o                                              \
            util/timer.o                                            \
            util/uid_generator.o                                    \
            util/uid.o                                              \
            video/sdl/sdl_painter.o                                 \
            video/sdl/sdl_renderer.o                                \
            video/sdl/sdl_surface_creator.o                         \
            video/sdl/sdl_texture.o                                 \
            video/sdl/sdl_video_system.o                            \
            video/sdl/sdlbased_video_system.o                       \
            video/canvas.o                                          \
            video/color.o                                           \
            video/compositor.o                                      \
            video/drawing_context.o                                 \
            video/font.o                                            \
            video/surface.o                                         \
            video/texture_manager.o                                 \
            video/texture.o                                         \
            video/ttf_font.o                                        \
            video/ttf_surface_manager.o                             \
            video/ttf_surface.o                                     \
            video/video_system.o                                    \
            video/viewport.o                                        \
            weapon/shooting_weapon/projectile/generic_projectile.o  \
            weapon/shooting_weapon/projectile/projectile_set.o      \
            weapon/shooting_weapon/projectile/projectile.o          \
            weapon/shooting_weapon/generic_shooting_weapon.o        \
            weapon/shooting_weapon/shooting_weapon.o                \
            weapon/weapon_set.o                                     \
            weapon/weapon.o                                         \

#CC specifies which compiler we're using
CC = g++

SDL2_PATH = D:/Libraries/SDL2/x86_64-w64-mingw32
SDL2_IMAGE_PATH = D:/Libraries/SDL2_image/x86_64-w64-mingw32
SDL2_MIXER_PATH = D:/Libraries/SDL2_mixer/x86_64-w64-mingw32
SDL2_TTF_PATH = D:/Libraries/SDL2_ttf/x86_64-w64-mingw32

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I$(SDL2_PATH)/include/SDL2          \
                -I$(SDL2_IMAGE_PATH)/include/SDL2    \
                -I$(SDL2_MIXER_PATH)/include/SDL2    \
                -I$(SDL2_TTF_PATH)/include/SDL2      \
                -I./src                              \

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L$(SDL2_PATH)/lib                   \
                -L$(SDL2_IMAGE_PATH)/lib             \
                -L$(SDL2_MIXER_PATH)/lib             \
                -L$(SDL2_TTF_PATH)/lib               \

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -std=c++17 -Wall -Wextra

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32           \
               -lSDL2main          \
               -lSDL2              \
               -lSDL2_image        \
               -lSDL2_mixer        \
               -lSDL2_ttf          \

GFLAGS = -g

OBJDIR := bin/obj
OBJS := $(addprefix $(OBJDIR)/, $(OBJ_FILES))

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = QBSKR
### Move the executable to ./data to (partial) fix the "../data/"
OBJ_NAME := $(addprefix ./data/, $(OBJ_NAME))

$(OBJDIR)/%.o : src/%.cpp
	@mkdir -p $(@D)
	$(CC) -c $(GFLAGS) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $< -o $@

#This is the target that compiles our executable
.PHONY : all
all: $(OBJ_NAME)

$(OBJ_NAME) : $(OBJS)
	$(CC) $(GFLAGS) $(OBJS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

$(OBJS): | $(OBJDIR)

.PHONY : clean
### idk why it can execute shell/bash (idk what it is even more)
### anyway...
clean:
	rm -rf ./bin/
	rm ./data/QBSKR.exe