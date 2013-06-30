
FLEX_TARGET(ConstParser src/parser_const.l  ${CMAKE_CURRENT_SOURCE_DIR}/src/parser_const.yy.cpp)

FILE(GLOB HOE_CORE_PUBLIC_HEADERS 
	../include/HoeCore/const_file.h
	../include/HoeCore/expression.h
	../include/HoeCore/hoe_core.h
	../include/HoeCore/hoe_file.h
	../include/HoeCore/hoe_fixed.h
	../include/HoeCore/hoe_flex.h
	../include/HoeCore/hoe_format.h
	../include/HoeCore/hoe_log.h
	../include/HoeCore/hoe_math.h
	../include/HoeCore/hoe_math.inl
	../include/HoeCore/hoe_math_def.h
	../include/HoeCore/hoe_math_ext.h
	../include/HoeCore/hoe_mem.h
	../include/HoeCore/hoe_platform.h
	../include/HoeCore/hoe_stream.h
	../include/HoeCore/hoe_string.h
	../include/HoeCore/hoe_structures.h
	../include/HoeCore/hoe_time.h
	../include/HoeCore/hoe_types.h
	../include/HoeCore/hoe_unicode.h
	../include/HoeCore/hoe_universal.h
	../include/HoeCore/map.h
	../include/HoeCore/struct_file.h)
SOURCE_GROUP("Header Files\\Public Headers" FILES ${HOE_CORE_PUBLIC_HEADERS})

FILE(GLOB HOE_CORE_SOURCE_PARSERS
	src/parser_const.l
	src/parser_struct.l
	src/parser_xml.l)
SOURCE_GROUP("Source Files\\Parsers" FILES ${HOE_CORE_SOURCE_PARSERS})

add_library(HoeCore 
	${HOE_CORE_PUBLIC_HEADERS}
	${HOE_CORE_SOURCE_PARSERS}
	src/StdAfx.h
	src/const_file.cpp
	src/expression.cpp
	src/hoe_core.cpp
	src/hoe_file.cpp
	src/hoe_fixed.cpp
	src/hoe_flex.cpp
	src/hoe_log.cpp
	src/hoe_math.cpp
	src/hoe_mem.cpp
	src/hoe_platform.cpp
	src/hoe_stream.cpp
	src/hoe_string.cpp
	src/hoe_time.cpp
	src/hoe_unicode.cpp
	src/hoe_universal.cpp
	src/struct_file.cpp
	${FLEX_ConstParser_OUTPUTS}
)
