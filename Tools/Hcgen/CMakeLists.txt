
add_executable(hcgen WIN32
  StdAfx.cpp
  StdAfx.h
  consts.cpp
  consts.h
  gen.cpp
  gen.h
  hcgen.cpp
)
target_link_libraries (hcgen HoeUtils HoeCore)

ADD_HOE_TOOL(hcgen)

