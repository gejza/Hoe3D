OUTPUT_DIR = ./bin
TMP_DIR = ./tmp

CC=gcc
GCC=g++
C_FLAGS= -D_LINUX -ggdb3 -I../Hoe3D/include -I../HoeGame
LINK=g++
L_FLAGS= -L/usr/X11R6/lib -L../bin
LINK_LIBRARIES = Common/tutorial.cpp -lHoeGame -llua -llualib -ldl -lX11 -lfl

all: bFirstApp bModels bPaint2D bInput bTerrain bLevel

bFirstApp:
	$(GCC) $(C_FLAGS) $(L_FLAGS) -o $(OUTPUT_DIR)/First FirstApp/first_app.cpp $(LINK_LIBRARIES)  	
bModels:
	$(GCC) $(C_FLAGS) $(L_FLAGS) -o $(OUTPUT_DIR)/Models Models/model_app.cpp $(LINK_LIBRARIES)  	
bPaint2D:
	$(GCC) $(C_FLAGS) $(L_FLAGS) -o $(OUTPUT_DIR)/Paint2D Paint2D/paint2d_app.cpp $(LINK_LIBRARIES) 
bInput:
	$(GCC) $(C_FLAGS) $(L_FLAGS) -o $(OUTPUT_DIR)/Input Input/input_app.cpp $(LINK_LIBRARIES) 
bTerrain:
	$(GCC) $(C_FLAGS) $(L_FLAGS) -o $(OUTPUT_DIR)/Terrain Terrain/terrain.cpp $(LINK_LIBRARIES)
bLevel:
	$(GCC) $(C_FLAGS) $(L_FLAGS) -o $(OUTPUT_DIR)/Level Level/level.cpp Level/bsp.cpp $(LINK_LIBRARIES)
