AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = ../bin/libHoeGame.la
#bin_PROGRAMS = ../bin/Hoe3D.so

src/config_parser.c: src/config_parser.l
	flex -o"src/config_parser.c" src/config_parser.l
src/lang_parser.c: src/lang_parser.l
	flex -o"src/lang_parser.c" src/lang_parser.l
src/figurefile_parser.cpp: src/figurefile_parser.l
	flex -o"src/figurefile_parser.cpp" src/figurefile_parser.l
	

___bin_libHoeGame_la_CPPFLAGS = -I../Hoe3D/include $(LUA_CFLAGS)

___bin_libHoeGame_la_SOURCES = src/hoe_app.cpp \
src/hoe_baseapp.cpp \
src/hoe_benchmark.cpp \
src/hoe_config.cpp \
src/hoe_console.cpp \
src/hoe_engine.cpp \
src/hoe_figure2d.cpp \
src/hoe_fs.cpp \
src/hoe_gui.cpp \
src/hoe_input.cpp \
src/hoe_inputkeys.cpp \
src/hoe_land.cpp \
src/hoe_lang.cpp \
src/hoe_linux.cpp \
src/hoe_music.cpp \
src/hoe_resource_mgr.cpp \
src/hoe_scene.cpp \
src/hoe_strategy.cpp \
src/hoe_stratmap.cpp \
src/hoe_timer.cpp \
src/hoe_utils.cpp \
src/hoe_vars.cpp \
src/hoe_view.cpp \
src/lua_script.cpp \
src/config_parser.c src/lang_parser.c src/figurefile_parser.cpp

___bin_libHoeGame_la_LDFLAGS = -module -lHoeCore


