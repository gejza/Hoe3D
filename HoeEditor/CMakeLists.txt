
# It was noticed that when using MinGW gcc it is essential that 'core' is mentioned before 'base'.
find_package(wxWidgets COMPONENTS core base REQUIRED)
FIND_PACKAGE(GTK2)
FIND_PACKAGE(GLIB2)
#SET(HOE3D_LIBS ${HOE3D_LIBS} ${GTK2_LIBRARIES})


INCLUDE_DIRECTORIES(
	${GTK2_INCLUDE_DIR}
	${GTK2_GDK_INCLUDE_DIR}
	${GTK2_ATK_INCLUDE_DIR}
	${GTK2_GDKCONFIG_INCLUDE_DIR}
	${GTK2_CAIRO_INCLUDE_DIR}
	${GTK2_PANGO_INCLUDE_DIR}
	${GTK2_GDK_PIXBUF_INCLUDE_DIR}
	${GLIB2_INCLUDE_DIRS})
##---------------------------------------------------
## Actual config file starts here
##---------------------------------------------------

# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )

set(HOE_EDITOR_SOURCE
  include/HoeEditor/app.h
  include/HoeEditor/base_console.h
  include/HoeEditor/editor.h
  include/HoeEditor/engine_console.h
  include/HoeEditor/engview.h
  include/HoeEditor/fs.h
  include/HoeEditor/log.h
  include/HoeEditor/panels.h
  include/HoeEditor/properties.h
  include/HoeEditor/resources.h
  include/HoeEditor/undolist.h
  include/HoeEditor/utils.h
  src/HoeEditor.rc
  src/StdAfx.h
  src/app.cpp
  src/base_console.cpp
  src/editor.cpp
  src/engine_console.cpp
  src/engview.cpp
  src/fs.cpp
  src/log.cpp
  src/panels.cpp
  src/properties.cpp
  src/resources.cpp
  src/undolist.cpp
  src/utils.cpp
)

if(WIN32)
 # set(HOE_EDITOR_SOURCE ${HOE_EDITOR_SOURCE} HoeEditor.rc)
endif(WIN32)

add_library(HoeEditor ${HOE_EDITOR_SOURCE})
target_link_libraries(HoeEditor ${wxWidgets_LIBRARIES})
