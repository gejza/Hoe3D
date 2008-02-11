
# mejkfajl k testovani zakladnich funkci core

all: string 
#	./math

math: math.cpp test.cpp
	g++ -o math -D_LINUX -D_NOXLIB math.cpp test.cpp 

struct: structures.cpp test.cpp
	g++ -o math -D_LINUX -D_NOXLIB structures.cpp test.cpp

string: string.cpp test.cpp ../../bin/libHoeCore.a
	g++ -o string -D_LINUX -D_NOXLIB string.cpp test.cpp -L../../bin -lHoeCore
