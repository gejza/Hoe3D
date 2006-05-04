#makefile

CC=gcc
GCC=g++
C_FLAGS= -D_LINUX -D_NOXLIB -I../../Hoe3D/include 
LINK=ar
L_FLAGS= cr

all: utils 

utils:  libhoeUtils.a

libhoeUtils.a:
	$(GCC) $(C_FLAGS) -c -o utils/utils.o utils/utils.cpp 
	$(LINK) $(L_FLAGS) ../../bin/libHoeUtils.a utils/utils.o

clean:
	rm -f libhoeCore.a libhoeUtils.a libhoeLevel.a
	rm -rf core/*.o
	rm -rf utils/*.o
	rm -rf level/*.o
 
