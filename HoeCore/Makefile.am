AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = ../bin/libHoeCore.la
#bin_PROGRAMS = ../bin/Hoe3D.so

#src/config_parser.cpp: src/config_parser.l
#	flex -o src/config_parser.cpp src/config_parser.l
#src/lang_parser.cpp: src/lang_parser.l
#	flex -o src/lang_parser.cpp src/lang_parser.l

___bin_libHoeCore_la_CPPFLAGS = -D_NOXLIB

___bin_libHoeCore_la_SOURCES = src/hoe_core.cpp \
src/hoe_log.cpp \
src/hoe_math.cpp

___bin_libHoeCore_la_LDFLAGS = -static

