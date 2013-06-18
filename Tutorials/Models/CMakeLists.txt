
add_executable(Models WIN32 ../Common/tutorial.h ../Common/tutorial.cpp model_app.cpp model_app.h)
target_link_libraries (Models HoeCore Hoe3D HoeGame ${LUA_LIBRARIES})

if(WIN32)
   set_target_properties(Models PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Models PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)