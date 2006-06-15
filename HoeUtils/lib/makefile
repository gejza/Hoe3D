#makefile

CC=gcc
GCC=g++
C_FLAGS= -D_LINUX -D_NOXLIB -I../../Hoe3D/include 
LINK=ar
L_FLAGS= cr

all: 
	$(GCC) $(C_FLAGS) -c -o utils.o utils.cpp 
	$(LINK) $(L_FLAGS) ../../bin/libHoeUtils.a utils.o

clean:
	rm -f llibHoeUtils.a libhoeLevel.a
 
