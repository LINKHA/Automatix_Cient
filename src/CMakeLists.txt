cmake_minimum_required(VERSION 3.20)

set (TARGET_NAME automatix_rt)

# Get all source files
file(GLOB_RECURSE SOURCE_HEADER *.h)
file(GLOB_RECURSE SOURCE_SRC *.cpp)
file(GLOB_RECURSE SOURCE_HPP *.hpp )
		
set(SOURCE_ALL_FILES ${SOURCE_HEADER} ${SOURCE_SRC} ${SOURCE_HPP})
source_group_by_dir(SOURCE_ALL_FILES)

include_directories (${AUTO_ROOT_PATH}/src)

add_library (${TARGET_NAME} ${AUTO_LIB_TYPE} ${SOURCE_ALL_FILES})