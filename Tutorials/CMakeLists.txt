#define a macro to simplify adding tests, then use it
MACRO(ADD_HOE_TUTORIAL name)
  #add_test (TutorialComp${arg} Tutorial ${arg})
  #set_tests_properties (TutorialComp${arg}
  #  PROPERTIES PASS_REGULAR_EXPRESSION ${result})
	SET_PROPERTY(TARGET ${name} PROPERTY FOLDER "Tutorials")
endmacro (ADD_HOE_TUTORIAL)

add_subdirectory (FirstApp)
add_subdirectory (Console)
add_subdirectory (Input)
add_subdirectory (Models)
add_subdirectory (Paint2D)
add_subdirectory (Sound)