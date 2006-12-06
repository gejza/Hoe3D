AUTOMAKE_OPTIONS = subdir-objects

#lib_LTLIBRARIES = ../bin/Hoe3D.so
bin_PROGRAMS = firstapp paint2D pathfind


firstapp_CPPFLAGS = -I../HoeCore/include -I../HoeGame/include -I../Hoe3D/include
firstapp_SOURCES = \ 
FirstApp/first_app.cpp \
Common/tutorial.cpp
firstapp_LDADD = -lHoeCore -lHoeGame -lbassmod $(LUA_LIBS) -lXxf86vm -lXxf86dga 

paint2D_CPPFLAGS = -I../HoeCore/include -I../HoeGame/include -I../Hoe3D/include
paint2D_SOURCES = \ 
Paint2D/paint_app.cpp \
Common/tutorial.cpp
paint2D_LDADD = -lHoeCore -lHoeGame -lbassmod $(LUA_LIBS) -lXxf86vm -lXxf86dga 


pathfind_CPPFLAGS = -I../HoeCore/include -I../HoeGame/include -I../Hoe3D/include
pathfind_SOURCES = \ 
PathFinding/pathfinding.cpp \
Common/tutorial.cpp
pathfind_LDADD =  -lHoeCore -lHoeGame -lbassmod $(LUA_LIBS) -lXxf86vm -lXxf86dga 



