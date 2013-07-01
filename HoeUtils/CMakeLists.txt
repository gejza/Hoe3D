
ADD_DEFINITIONS(-D_NOXLIB)

add_library(HoeUtils
  ../include/HoeUtils/error.h
  ../include/HoeUtils/hoe_utils.h
  ../include/HoeUtils/text.h
  src/StdAfx.cpp
  src/StdAfx.h
  src/error.cpp
  src/hoe_utils.cpp
  src/text.cpp
)
