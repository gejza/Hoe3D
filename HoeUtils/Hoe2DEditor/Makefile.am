
# these are the headers for your project
# noinst_HEADERS = wokno.h

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = ../bin/Hoe2DEditor

# the application source, library search path, and link libraries
___bin_Hoe2DEditor_SOURCES = \ 
editor.cpp  figure.cpp  items.cpp  tools.cpp


___bin_Hoe2DEditor_CPPFLAGS = $(WX_CPPFLAGS) -I../../HoeCore/include -I../../Hoe3D/include -I../../HoeGame/include  -I../../HoeEditor/include 

___bin_Hoe2DEditor_LDFLAGS = -L../../bin -ll -lHoeGame -lHoeCore -lHoeEditor -lHoeUtils -ldl -lX11 -lfl $(WX_LIB) -lwx_gtk2_propgrid-2.8 $(LUA_LIBS)

