### After some scraping online, I have C-c C-v -ed the Makefile that only compile what needed to compile
### It's not fully fool-proof because of no dependency check
### So I will delete definition code in *.hpp files :D

CPP_FILES = main.cpp                                                  \
            audio/sdl_mix_chunk.cpp                                   \
            audio/sdl_mix_music.cpp                                   \
            audio/sound_manager.cpp                                   \
            badguy/badguy_set.cpp                                     \
            badguy/badguy.cpp                                         \
            badguy/balbusour.cpp                                      \
            badguy/generic_badguy.cpp                                 \
            collision/collision_object.cpp                            \
            collision/collision_system.cpp                            \
            collision/collision.cpp                                   \
            control/controller.cpp                                    \
            control/input_manager.cpp                                 \
            control/keyboard_config.cpp                               \
            control/keyboard_manager.cpp                              \
            control/mouse_button_config.cpp                           \
            control/mouse_button_manager.cpp                          \
            control/mouse_motion_manager.cpp                          \
            gui/item_action.cpp                                       \
            gui/item_horizontal_line.cpp                              \
            gui/item_label.cpp                                        \
            gui/menu_item.cpp                                         \
            gui/menu_manager.cpp                                      \
            gui/menu_set.cpp                                          \
            gui/menu_transition.cpp                                   \
            gui/menu.cpp                                              \
            gui/mouse_cursor.cpp                                      \
            math/interpolate.cpp                                      \
            math/random.cpp                                           \
            math/rect.cpp                                             \
            math/rectf.cpp                                            \
            math/size.cpp                                             \
            math/sizef.cpp                                            \
            math/util.cpp                                             \
            math/vector.cpp                                           \
            object/camera.cpp                                         \
            object/credit_scroller.cpp                                \
            object/floating_text.cpp                                  \
            object/game_object_manager.cpp                            \
            object/game_object.cpp                                    \
            object/info_box_line.cpp                                  \
            object/moving_object.cpp                                  \
            object/moving_sprite.cpp                                  \
            object/physic.cpp                                         \
            object/player.cpp                                         \
            object/tile_map.cpp                                       \
            object/tile_set_parser.cpp                                \
            object/tile_set.cpp                                       \
            object/tile.cpp                                           \
            qbskr/menu/game_menu.cpp                                  \
            qbskr/menu/main_menu.cpp                                  \
            qbskr/color_scheme.cpp                                    \
            qbskr/constants.cpp                                       \
            qbskr/credit_screen.cpp                                   \
            qbskr/gameconfig.cpp                                      \
            qbskr/game_session.cpp                                    \
            qbskr/globals.cpp                                         \
            qbskr/level_data_set.cpp                                  \
            qbskr/level_data.cpp                                      \
            qbskr/level.cpp                                           \
            qbskr/main.cpp                                            \
            qbskr/resources.cpp                                       \
            qbskr/room_data_set.cpp                                   \
            qbskr/room_data.cpp                                       \
            qbskr/room.cpp                                            \
            qbskr/screen_fade.cpp                                     \
            qbskr/screen_manager.cpp                                  \
            qbskr/screen.cpp                                          \
            qbskr/title_screen.cpp                                    \
            qbskr/waiting_screen.cpp                                  \
            sprite/sprite_data.cpp                                    \
            sprite/sprite_manager.cpp                                 \
            sprite/sprite.cpp                                         \
            util/crappy_reader_data.cpp                               \
            util/crappy_reader.cpp                                    \
            util/line_iterator.cpp                                    \
            util/log.cpp                                              \
            util/string_util.cpp                                      \
            util/timer.cpp                                            \
            util/uid_generator.cpp                                    \
            util/uid.cpp                                              \
            video/sdl/sdl_painter.cpp                                 \
            video/sdl/sdl_renderer.cpp                                \
            video/sdl/sdl_surface_creator.cpp                         \
            video/sdl/sdl_texture.cpp                                 \
            video/sdl/sdl_video_system.cpp                            \
            video/sdl/sdlbased_video_system.cpp                       \
            video/canvas.cpp                                          \
            video/color.cpp                                           \
            video/compositor.cpp                                      \
            video/drawing_context.cpp                                 \
            video/font.cpp                                            \
            video/surface.cpp                                         \
            video/texture_manager.cpp                                 \
            video/texture.cpp                                         \
            video/ttf_font.cpp                                        \
            video/ttf_surface_manager.cpp                             \
            video/ttf_surface.cpp                                     \
            video/video_system.cpp                                    \
            video/viewport.cpp                                        \
            weapon/shooting_weapon/projectile/generic_projectile.cpp  \
            weapon/shooting_weapon/projectile/projectile_set.cpp      \
            weapon/shooting_weapon/projectile/projectile.cpp          \
            weapon/shooting_weapon/generic_shooting_weapon.cpp        \
            weapon/shooting_weapon/shooting_weapon.cpp                \
            weapon/weapon_set.cpp                                     \
            weapon/weapon.cpp                                         \

#CC specifies which compiler we're using
CC = g++

ifdef OS
    # only Windows define %OS% aparrently (https://stackoverflow.com/a/4511164)
    ### Hardcoding paths
    ### if want compile then can't 
    SDL2_PATH = D:/Libraries/SDL2/x86_64-w64-mingw32
    SDL2_IMAGE_PATH = D:/Libraries/SDL2_image/x86_64-w64-mingw32
    SDL2_MIXER_PATH = D:/Libraries/SDL2_mixer/x86_64-w64-mingw32
    SDL2_TTF_PATH = D:/Libraries/SDL2_ttf/x86_64-w64-mingw32

    # For myself
    INCLUDE_PATHS = -I$(SDL2_PATH)/include/SDL2          \
                    -I$(SDL2_IMAGE_PATH)/include/SDL2    \
                    -I$(SDL2_MIXER_PATH)/include/SDL2    \
                    -I$(SDL2_TTF_PATH)/include/SDL2      \
                    -I./src                              \
    
    # For myself
    LIBRARY_PATHS = -L$(SDL2_PATH)/lib                   \
                    -L$(SDL2_IMAGE_PATH)/lib             \
                    -L$(SDL2_MIXER_PATH)/lib             \
                    -L$(SDL2_TTF_PATH)/lib               \

    # For Windows
    LINKER_FLAGS = -lmingw32              \
                   -lSDL2main             \
                   -lSDL2                 \
                   -lSDL2_image           \
                   -lSDL2_mixer           \
                   -lSDL2_ttf             \

else
    # from here assume it's Linux
    # otherwise not considered

    #Include paths for Linux (Ubuntu 22.04)
    # "-I/usr/include/SDL2/" = fix to so that I don't have to changed every #include <SDL.h>
    INCLUDE_PATHS = -I/usr/include/SDL2/  \
                    -I./src/              \

    #Library paths for Linux (Ubuntu 22.04)
    # = none
    LIBRARY_PATHS = 

    #Linker flags for Linux (Ubuntu 22.04)
    LINKER_FLAGS = -lSDL2main             \
                   -lSDL2                 \
                   -lSDL2_image           \
                   -lSDL2_mixer           \
                   -lSDL2_ttf             \

endif



#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -std=c++17 -Wall -Wextra

GFLAGS = -g

OBJ_FILES = $(addsuffix .o, $(basename $(CPP_FILES)))

OBJDIR := bin/obj
OBJS := $(addprefix $(OBJDIR)/, $(OBJ_FILES))

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = QBSKR.exe
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

$(OBJDIR) :
	@mkdir -p $(OBJDIR)

.PHONY : clean
### idk why it can execute shell/bash (idk what it is even more)
### anyway...
clean:
	rm -rf ./bin/obj/
	rm ./data/QBSKR.exe