AUTOMAKE_OPTIONS = subdir-objects

#lib_LTLIBRARIES = ../bin/Hoe3D.so
bin_PROGRAMS = ../bin/FirstApp


___bin_FirstApp_CPPFLAGS = -I../../HoeCore/include -I../../HoeGame/include -I../../Hoe3D/include
___bin_FirstApp_SOURCES = \ 
first_app.cpp \
../Common/tutorial.cpp
___bin_FirstApp_LDADD = -L../../bin -lHoeGame -lHoeCore  $(LUA_LIBS) -lXxf86vm -lXxf86dga  




