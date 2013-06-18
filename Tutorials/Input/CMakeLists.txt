
add_executable(Input WIN32 ../Common/tutorial.h ../Common/tutorial.cpp input_app.cpp input_app.h)
target_link_libraries (Input HoeCore Hoe3D HoeGame ${LUA_LIBRARIES})

if(WIN32)
   #set_target_properties(Input PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   #set_target_properties(Input PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   #set_target_properties(Input PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   #set_target_properties(Input PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(Input PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(Input PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)