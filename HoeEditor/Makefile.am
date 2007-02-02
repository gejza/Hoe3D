AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = ../bin/libHoeEditor.la
#bin_PROGRAMS = ../bin/Hoe3D.so

src/config_parser.c: src/config_parser.l
	flex -o"src/config_parser.c" src/config_parser.l
src/lang_parser.c: src/lang_parser.l
	flex -o"src/lang_parser.c" src/lang_parser.l
src/figurefile_parser.cpp: src/figurefile_parser.l
	flex -o"src/figurefile_parser.cpp" src/figurefile_parser.l
	

___bin_libHoeEditor_la_CPPFLAGS = -I../Hoe3D/include -I../HoeGame/include $(WX_CPPFLAGS) $(GTK_CFLAGS) $(WX_CXXFLAGS_ONLY)

___bin_libHoeEditor_la_SOURCES = src/app.cpp \
src/engview.cpp src/editor.cpp \
src/base_console.cpp \
src/undolist.cpp \
src/properties.cpp src/utils.cpp \
src/panels.cpp src/resources.cpp src/log.cpp src/fs.cpp

___bin_libHoeEditor_la_LDFLAGS = -static -lHoeCore -lHoeGame $(WX_LIBS)


