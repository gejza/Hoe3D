#define a macro to simplify adding tests, then use it
MACRO(ADD_HOE_TOOL name)
  #add_test (TutorialComp${arg} Tutorial ${arg})
  #set_tests_properties (TutorialComp${arg}
  #  PROPERTIES PASS_REGULAR_EXPRESSION ${result})
	SET_PROPERTY(TARGET ${name} PROPERTY FOLDER "Tools")
endmacro (ADD_HOE_TOOL)

add_subdirectory (Hcgen)
add_subdirectory (Hxres)
