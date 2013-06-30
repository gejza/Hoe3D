
add_executable(hxres WIN32
  StdAfx.cpp
  StdAfx.h
  comp_font.cpp
  comp_font.h
  compiler.cpp
  compiler.h
  hxres.cpp
  image.cpp
  image.h
  linker.cpp
  linker.h
  linkout.cpp
  linkout.h
  parse.h
  scan.h
)
#target_link_libraries (FirstApp HoeCore Hoe3D HoeGame ${LUA_LIBRARIES})

ADD_HOE_TOOL(hxres)
