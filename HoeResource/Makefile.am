AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = ../bin/libHoeResource.la

___bin_libHoeResource_la_CPPFLAGS = -D_NOXLIB -I../HoeCore/include

___bin_libHoeResource_la_SOURCES = src/fvf.cpp src/hoe_resfile.cpp \
								   src/hoe_res.cpp \
								   src/hoe_png.cpp src/hoe_jpg.cpp
#src/hsl_parser.tab.cpp src/hsl_parser.yy.cpp

___bin_libHoeResource_la_LDFLAGS = -static -lHoeCore


