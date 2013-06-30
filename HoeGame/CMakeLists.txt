
FLEX_TARGET(ConfigParser src/config_parser.l  ${CMAKE_CURRENT_SOURCE_DIR}/src/config_parser.yy.cpp)
FLEX_TARGET(LangParser src/lang_parser.l  ${CMAKE_CURRENT_SOURCE_DIR}/src/lang_parser.yy.cpp)

SET(HOE_GAME_SOURCE
src/StdAfx.h
src/hoe_adventure.cpp
src/hoe_ai.cpp
src/hoe_app.cpp
src/hoe_baseapp.cpp
src/hoe_benchmark.cpp
src/hoe_config.cpp
src/hoe_console.cpp
src/hoe_engine.cpp
src/hoe_figure2d.cpp
src/hoe_fs.cpp
src/hoe_gui.cpp
src/hoe_input.cpp
src/hoe_inputkeys.cpp
src/hoe_land.cpp
src/hoe_lang.cpp
src/hoe_mem.cpp
src/hoe_music.cpp
src/hoe_resource_mgr.cpp
src/hoe_scene.cpp
src/hoe_strategy.cpp
src/hoe_stratmap.cpp
src/hoe_structfile.cpp
src/hoe_timer.cpp
src/hoe_utils.cpp
src/hoe_vars.cpp
src/hoe_view.cpp
src/lua_script.cpp
src/lang_parser.l
src/config_parser.l
src/structfile_parser.l)

# src/hoe_linux.cpp
# src/hoe_macosx.cpp
# src/hoe_mobile.cpp

# src/hoe_settings.cpp
IF(WIN32)
	SET(HOE_GAME_SOURCE ${HOE_GAME_SOURCE} src/hoe_win32.cpp src/hoe_wx.cpp)
ENDIF()

IF(UNIX)
	SET(HOE_GAME_SOURCE ${HOE_GAME_SOURCE} src/hoe_linux.cpp)
ENDIF()

add_library(HoeGame
	${HOE_GAME_SOURCE}
	${FLEX_ConfigParser_OUTPUTS}
	${FLEX_LangParser_OUTPUTS})

