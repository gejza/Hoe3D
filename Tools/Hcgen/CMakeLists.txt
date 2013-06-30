
add_executable(hcgen WIN32
  StdAfx.cpp
  StdAfx.h
  consts.cpp
  consts.h
  gen.cpp
  gen.h
  hcgen.cpp
)
target_link_libraries (hcgen HoeCore HoeUtils)

ADD_HOE_TOOL(hcgen)

