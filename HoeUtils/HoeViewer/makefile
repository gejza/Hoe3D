# makefile

GCC=g++
C_FLAGS= -D_LINUX -D_NOXLIB -I../../../Hoe3D/include -I../../sdk
LINK=g++
L_FLAGS= -shared -fPIC -L/usr/X11R6/lib -L../../sdk/
LINK_LIBRARIES = -lhoeCore -lhoeUtils

all:
	$(LINK) $(C_FLAGS) $(L_FLAGS) -o ../TextureView.so textureview.cpp  $(LINK_LIBRARIES)

                                               
