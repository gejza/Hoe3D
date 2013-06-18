
add_executable(Sound WIN32 ../Common/tutorial.h ../Common/tutorial.cpp sound_app.cpp sound_app.h)
target_link_libraries (Sound HoeCore Hoe3D HoeGame ${LUA_LIBRARIES})

if(WIN32)
   set_target_properties(Sound PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Sound PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)