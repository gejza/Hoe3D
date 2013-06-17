
FLEX_TARGET(ConstParser src/parser_const.l  ${CMAKE_CURRENT_SOURCE_DIR}/src/parser_const.yy.cpp)

add_library(HoeCore src/StdAfx.h
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
src/parser_const.l
src/parser_struct.l
src/parser_xml.l
#src/parser_const.yy.cpp
#src/parser_struct.yy.cpp
#src/parser_xml.yy.cpp
${FLEX_ConstParser_OUTPUTS})

