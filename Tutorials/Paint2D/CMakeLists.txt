
add_executable(Paint2D WIN32 ../Common/tutorial.h ../Common/tutorial.cpp paint2d_app.cpp paint2d_app.h)
target_link_libraries (Paint2D HoeGame Hoe3D HoeCore ${LUA_LIBRARIES} dl ${X11_X11_LIB} ${X11_Xxf86vm_LIB} Xxf86dga)

if(WIN32)
   set_target_properties(Paint2D PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Paint2D PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)

ADD_HOE_TUTORIAL(Paint2D)
