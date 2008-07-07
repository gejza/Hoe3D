
# these are the headers for your project
# noinst_HEADERS = wokno.h

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = hcgen

# the application source, library search path, and link libraries
hcgen_SOURCES = hcgen.cpp consts.cpp gen.cpp 

hcgen_CPPFLAGS = -D_NOXLIB -I../../HoeCore/include/HoeCore -I../../HoeUtils/include -I../../HoeResource/include

hcgen_LDFLAGS = -L../../bin -ll -lHoeUtils -lHoeResource -lHoeCore


