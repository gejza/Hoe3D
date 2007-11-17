AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = ../bin/libHoeResource.la
#bin_PROGRAMS = ../bin/Hoe3D.so

src/hsl_parser.yy.cpp: src/hsl_parser.l
	flex -o"src/hsl_parser.yy.cpp" src/hsl_parser.l
src/hsl_parser.tab.cpp: src/hsl_parser.y
	bison  -d -o"src/hsl_parser.tab.cpp" src/hsl_parser.y
	

___bin_libHoeResource_la_CPPFLAGS = -I../Hoe3D/include $(LUA_CFLAGS)

___bin_libHoeResource_la_SOURCES = \
src/hsl_parser.tab.cpp src/hsl_parser.yy.cpp

___bin_libHoeResource_la_LDFLAGS = -static -lHoeCore


