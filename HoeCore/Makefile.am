AUTOMAKE_OPTIONS = subdir-objects

lib_LIBRARIES = ../bin/libHoeCore.a
noinst_PROGRAMS = HoeCoreTest

#src/config_parser.cpp: src/config_parser.l
#	flex -o src/config_parser.cpp src/config_parser.l
#src/lang_parser.cpp: src/lang_parser.l
#	flex -o src/lang_parser.cpp src/lang_parser.l

___bin_libHoeCore_a_CPPFLAGS = -D_NOXLIB -fPIC

___bin_libHoeCore_a_SOURCES = src/hoe_core.cpp \
src/hoe_log.cpp \
src/hoe_math.cpp \
src/hoe_string.cpp \
src/hoe_universal.cpp

___bin_libHoeCore_a_LDFLAGS = -static

HoeCoreTest_SOURCES = test/coretest.cpp



