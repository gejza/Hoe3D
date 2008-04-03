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
src/hoe_universal.cpp \
src/hoe_fixed.cpp \
src/hoe_mem.cpp \
src/hoe_stream.cpp \
src/hoe_file.cpp \
src/hoe_time.cpp \
src/hoe_unicode.cpp \
src/hoe_flex.cpp \
src/hoe_platform.cpp \
src/expression.cpp \
src/const_file.cpp \
src/parser_const.yy.cpp \
src/parser_struct.yy.cpp

#src/parser_xml.yy.cpp 
___bin_libHoeCore_a_LDFLAGS = -static

src/parser_const.yy.cpp: src/parser_const.l template/hoe_flex.skl
	flex -Stemplate/hoe_flex.skl -osrc/parser_const.yy.cpp src/parser_const.l

src/parser_struct.yy.cpp: src/parser_struct.l template/hoe_flex.skl
	flex -Stemplate/hoe_flex.skl -osrc/parser_struct.yy.cpp src/parser_struct.l

src/parser_xml.yy.cpp: src/parser_xml.l template/hoe_flex.skl
	flex -Stemplate/hoe_flex.skl -osrc/parser_xml.yy.cpp src/parser_xml.l

HoeCoreTest_SOURCES = test/test.cpp test/string.cpp

HoeCoreTest_CPPFLAGS = -D_NOXLIB 

HoeCoreTest_LDFLAGS = -L../bin -lHoeCore

test/flex.cpp: template/hoe_flex.skl test/flex.l
	flex -Stemplate/hoe_flex.skl -otest/flex.cpp test/flex.l

TestDynString: HoeCoreTest
	./HoeCoreTest DynString

