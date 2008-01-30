AUTOMAKE_OPTIONS = subdir-objects

lib_LIBRARIES = ../bin/libHoeGame.a
#bin_PROGRAMS = ../bin/Hoe3D.so

src/config_parser.cpp: src/config_parser.l
	flex -o"src/config_parser.cpp" src/config_parser.l
src/lang_parser.cpp: src/lang_parser.l
	flex -o"src/lang_parser.cpp" src/lang_parser.l
src/structfile_parser.cpp: src/structfile_parser.l
	flex -o"src/structfile_parser.cpp" src/structfile_parser.l
	

___bin_libHoeGame_a_CPPFLAGS = -I../Hoe3D/include $(LUA_CFLAGS) -DHOE_NO_BASS -DHOE_STATIC_ENGINE

___bin_libHoeGame_a_SOURCES = src/hoe_app.cpp \
src/hoe_baseapp.cpp \
src/hoe_benchmark.cpp \
src/hoe_config.cpp \
src/hoe_console.cpp \
src/hoe_engine.cpp \
src/hoe_figure2d.cpp \
src/hoe_gui.cpp \
src/hoe_input.cpp \
src/hoe_inputkeys.cpp \
src/hoe_land.cpp \
src/hoe_lang.cpp \
src/hoe_linux.cpp \
src/hoe_music.cpp \
src/hoe_resource_mgr.cpp \
src/hoe_scene.cpp \
src/hoe_adventure.cpp \
src/hoe_strategy.cpp \
src/hoe_stratmap.cpp \
src/hoe_timer.cpp \
src/hoe_utils.cpp \
src/hoe_vars.cpp \
src/hoe_view.cpp \
src/hoe_mem.cpp \
src/hoe_structfile.cpp 

#src/lua_script.cpp \
#src/config_parser.cpp src/lang_parser.cpp src/structfile_parser.cpp
___bin_libHoeGame_a_LDFLAGS = -static -lHoeCore


