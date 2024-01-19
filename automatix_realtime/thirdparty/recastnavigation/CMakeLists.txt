cmake_minimum_required(VERSION 3.20)
set(TARGET_NAME recastnavigation)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/Detour/Include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/DetourCrowd/Include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/DetourTileCache/Include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/Recast/Include)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_HEADER *.h)
file(GLOB_RECURSE SOURCE_SRC *.cpp)
file(GLOB_RECURSE SOURCE_HPP *.hpp )
file(GLOB_RECURSE SOURCE_SRC_C *.c )

set(SOURCE_ALL_FILES ${SOURCE_HEADER} ${SOURCE_SRC} ${SOURCE_HPP} ${SOURCE_SRC_C} )
source_group_by_dir(SOURCE_ALL_FILES)


add_library(${TARGET_NAME} ${SOURCE_ALL_FILES})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "ThirdParty") 
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE cxx) 