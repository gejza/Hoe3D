AUTOMAKE_OPTIONS = subdir-objects

#lib_LTLIBRARIES = ../bin/Hoe3D.so
bin_PROGRAMS = ../bin/Console


___bin_Console_CPPFLAGS = -I../../HoeCore/include -I../../HoeGame/include -I../../Hoe3D/include
___bin_Console_SOURCES = \ 
console_app.cpp \
../Common/tutorial.cpp
___bin_Console_LDADD = -L../../bin -lHoeCore -lHoeGame $(LUA_LIBS) -lXxf86vm -lXxf86dga  




