cmake_minimum_required(VERSION 3.20)
set(TARGET_NAME lua)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SOURCE_SRC onelua.c)
set(SOURCE_ALL_FILES ${SOURCE_SRC})
source_group_by_dir(SOURCE_ALL_FILES)


add_library(${TARGET_NAME} ${SOURCE_ALL_FILES})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE cxx) 
target_compile_definitions(${TARGET_NAME} PRIVATE MAKE_LIB)