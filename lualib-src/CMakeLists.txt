cmake_minimum_required(VERSION 3.20)
set(TARGET_NAME lualib)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${AUTO_ROOT_PATH}/thirdparty)

file(GLOB_RECURSE SOURCE_HEADER *.h)
file(GLOB_RECURSE SOURCE_SRC *.cpp)
set(SOURCE_ALL_FILES ${SOURCE_SRC})
source_group_by_dir(SOURCE_ALL_FILES)

add_library(${TARGET_NAME} ${SOURCE_ALL_FILES})
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE cxx) 