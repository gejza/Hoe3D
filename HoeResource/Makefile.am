AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = ../bin/libHoeResource.la
bin_PROGRAMS = test_flex

src/test.yy.cpp: src/test.l ../HoeCore/template/hoe_flex.skl
	flex -S ../HoeCore/template/hoe_flex.skl  -o"src/test.yy.cpp" src/test.l

#src/hsl_parser.yy.cpp: src/hsl_parser.l
#	flex -o"src/hsl_parser.yy.cpp" src/hsl_parser.l
#src/hsl_parser.tab.cpp: src/hsl_parser.y
#	bison  -d -o"src/hsl_parser.tab.cpp" src/hsl_parser.y
	

___bin_libHoeResource_la_CPPFLAGS = -I../Hoe3D/include $(LUA_CFLAGS)

___bin_libHoeResource_la_SOURCES = src/test.yy.cpp
#src/hsl_parser.tab.cpp src/hsl_parser.yy.cpp

___bin_libHoeResource_la_LDFLAGS = -static -lHoeCore


test_flex_SOURCES = src/test.yy.cpp
