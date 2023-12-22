set (TARGET_NAME automatix_realtime)

# add include path
include_directories(${AUTO_ROOT_PATH}/automatix_realtime)
include_directories(${AUTO_ROOT_PATH}/thirdparty)
include_directories(${AUTO_ROOT_PATH}/thirdparty/ylt/thirdparty)

# Get all source files
file(GLOB_RECURSE SOURCE_HEADER *.h)
file(GLOB_RECURSE SOURCE_SRC *.cpp)
file(GLOB_RECURSE SOURCE_HPP *.hpp )
file(GLOB_RECURSE SOURCE_SRC_C *.c )
		
set(SOURCE_ALL_FILES ${SOURCE_HEADER} ${SOURCE_SRC} ${SOURCE_HPP} ${SOURCE_SRC_C} )
source_group_by_dir(SOURCE_ALL_FILES)

add_executable(${TARGET_NAME} ${SOURCE_ALL_FILES})