
add_executable(FirstApp WIN32 ../Common/tutorial.h ../Common/tutorial.cpp first_app.cpp first_app.h)
target_link_libraries (FirstApp HoeCore Hoe3D HoeGame ${LUA_LIBRARIES})

if(WIN32)
   #set_target_properties(FirstApp PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
   #set_target_properties(FirstApp PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
   #set_target_properties(FirstApp PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
   #set_target_properties(FirstApp PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
   set_target_properties(FirstApp PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(FirstApp PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(WIN32)