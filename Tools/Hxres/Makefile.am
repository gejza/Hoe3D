
# these are the headers for your project
# noinst_HEADERS = wokno.h

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = hxres

# the application source, library search path, and link libraries
hxres_SOURCES = hxres.cpp parse.tab.cpp scan.yy.cpp linker.cpp compiler.cpp image.cpp error.cpp linkout.cpp comp_font.cpp

hxres_CPPFLAGS = -D_NOXLIB -I../../HoeCore/include -I../../HoeUtils/include -I../../HoeResource/include

hxres_LDFLAGS = -L../../bin -ll -lHoeUtils -lHoeResource -lHoeCore -ldl -ljpeg -lpng 

scan.yy.cpp: scan.l ../../HoeCore/template/hoe_flex.skl
	flex -S ../../HoeCore/template/hoe_flex.skl -o scan.yy.cpp scan.l

parse.tab.cpp: parse.y
	bison -d -o parse.tab.cpp parse.y

