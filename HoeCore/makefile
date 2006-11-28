OUTPUT_DIR = ../bin/
TMP_DIR = tmp

OUTPUT = $(OUTPUT_DIR)/libHoeCore.a

CC=gcc
GCC=g++
C_FLAGS= -mmmx -msse -march=pentium3  -D_LINUX -D_NOXLIB -ggdb3
LINK=ar
L_FLAGS= cr

OBJ_FILES = $(TMP_DIR)/hoe_log.o $(TMP_DIR)/hoe_core.o $(TMP_DIR)/hoe_math.o

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


