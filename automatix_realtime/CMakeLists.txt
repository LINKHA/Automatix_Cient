set (TARGET_NAME automatix_realtime)

# add include path
include_directories(${AUTO_ROOT_PATH}/automatix_realtime)
include_directories(${AUTO_ROOT_PATH}/thirdparty)

set(CMAKE_C_STANDARD 11)
# Get all source files
file(GLOB_RECURSE SOURCE_HEADER *.h)
file(GLOB_RECURSE SOURCE_SRC *.cpp)
file(GLOB_RECURSE SOURCE_HPP *.hpp )
file(GLOB_RECURSE SOURCE_SRC_C *.c )
		
set(SOURCE_ALL_FILES ${SOURCE_HEADER} ${SOURCE_SRC} ${SOURCE_HPP} ${SOURCE_SRC_C} )
source_group_by_dir(SOURCE_ALL_FILES)

if(MSVC)
    add_compile_options(/experimental:c11atomics)
endif()

add_executable(${TARGET_NAME} ${SOURCE_ALL_FILES})

target_link_libraries (${TARGET_NAME} lua)
include_directories (${AUTO_ROOT_PATH}/thirdparty/lua)

target_link_libraries (${TARGET_NAME} kcp)
include_directories (${AUTO_ROOT_PATH}/thirdparty/kcp)

target_link_libraries (${TARGET_NAME} yyjson)
include_directories (${AUTO_ROOT_PATH}/thirdparty/yyjson)

target_link_libraries (${TARGET_NAME} recastnavigation)
include_directories (${AUTO_ROOT_PATH}/thirdparty/recastnavigation)
include_directories (${AUTO_ROOT_PATH}/thirdparty/recastnavigation/Detour/Include)
include_directories (${AUTO_ROOT_PATH}/thirdparty/recastnavigation/DetourCrowd/Include)
include_directories (${AUTO_ROOT_PATH}/thirdparty/recastnavigation/DetourTileCache/Include)
include_directories (${AUTO_ROOT_PATH}/thirdparty/recastnavigation/Recast/Include)

target_link_libraries (${TARGET_NAME} fastlz)
include_directories (${AUTO_ROOT_PATH}/thirdparty/fastlz)