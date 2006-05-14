OUTPUT_DIR = ../bin/
TMP_DIR = tmp

OUTPUT = $(OUTPUT_DIR)/libHoeGame.a

CC=gcc
GCC=g++
C_FLAGS= -D_LINUX -ggdb3 -I../Hoe3D/include
LINK=ar
L_FLAGS= cr

C_FILES = dictionary.c strlib.c iniparser.c lang_parser.c
CPP_FILES = hoe_resource_mgr.cpp hoe_console.cpp hoe_lang.cpp hoe_linux.cpp \
hoe_app.cpp hoe_settings.cpp hoe_baseapp.cpp hoe_engine.cpp hoe_figure2d.cpp \
hoe_view.cpp hoe_utils.cpp hoe_gui.cpp hoe_inputkeys.cpp

OBJ_FILES = $(TMP_DIR)/hoe_resource_mgr.o $(TMP_DIR)/hoe_console.o $(TMP_DIR)/hoe_lang.o $(TMP_DIR)/hoe_linux.o $(TMP_DIR)/dictionary.o $(TMP_DIR)/strlib.o \
$(TMP_DIR)/iniparser.o $(TMP_DIR)/lang_parser.o $(TMP_DIR)/hoe_settings.o $(TMP_DIR)/hoe_baseapp.o $(TMP_DIR)/hoe_engine.o $(TMP_DIR)/hoe_app.o $(TMP_DIR)/hoe_figure2d.o \
$(TMP_DIR)/hoe_view.o $(TMP_DIR)/hoe_utils.o $(TMP_DIR)/hoe_gui.o $(TMP_DIR)/hoe_inputkeys.o $(TMP_DIR)/hoe_strategy.o $(TMP_DIR)/hoe_stratmap.o \
$(TMP_DIR)/lua_script.o $(TMP_DIR)/hoe_scene.o $(TMP_DIR)/hoe_input.o $(TMP_DIR)/hoe_fs.o \
$(TMP_DIR)/hoe_timer.o $(TMP_DIR)/hoe_vars.o $(TMP_DIR)/hoe_config.o \
$(TMP_DIR)/config_parser.o $(TMP_DIR)/hoe_music.o $(TMP_DIR)/hoe_benchmark.o


all:	BUILD

$(TMP_DIR):
	mkdir -p $(TMP_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

BUILD:		$(TMP_DIR) $(OUTPUT_DIR) $(OBJ_FILES) 
	$(LINK) $(L_FLAGS) $(OUTPUT) $(OBJ_FILES)


clean:
	rm -f $(OBJ_FILES) $(OUTPUT_BINARY)

#
# Generic rules to build object files
#

$(TMP_DIR)/%.o:		src/%.cpp
	$(GCC) $(C_FLAGS) -c $< -o $@

$(TMP_DIR)/%.o:		src/%.c
	$(CC) $(C_FLAGS) -c $< -o $@


