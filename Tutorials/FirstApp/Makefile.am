AUTOMAKE_OPTIONS = subdir-objects

#lib_LTLIBRARIES = ../bin/Hoe3D.so
bin_PROGRAMS = FirstApp


FirstApp_CPPFLAGS = -I../../HoeCore/include -I../../HoeGame/include -I../../Hoe3D/include -I../../HoeResource/include
FirstApp_SOURCES = first_app.cpp ../Common/tutorial.cpp
FirstApp_LDADD = -L../../bin -lHoeGame -lHoeCore  $(LUA_LIBS) -lXxf86vm -lXxf86dga -lX11 -ldl




