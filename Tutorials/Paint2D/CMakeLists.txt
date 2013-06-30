
add_executable(Paint2D WIN32 ../Common/tutorial.h ../Common/tutorial.cpp paint2d_app.cpp paint2d_app.h)
target_link_libraries (Paint2D HoeCore Hoe3D HoeGame ${LUA_LIBRARIES})

if(WIN32)
   set_target_properties(Paint2D PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Paint2D PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)

ADD_HOE_TUTORIAL(Paint2D)