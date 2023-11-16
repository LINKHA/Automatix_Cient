set (TARGET_NAME automatix_realtime)

# Get all source files
file(GLOB_RECURSE SOURCE_HEADER *.h)
file(GLOB_RECURSE SOURCE_SRC *.cpp)
file(GLOB_RECURSE SOURCE_HPP *.hpp )
		
set(SOURCE_ALL_FILES ${SOURCE_HEADER} ${SOURCE_SRC} ${SOURCE_HPP})
source_group_by_dir(SOURCE_ALL_FILES)

include_directories (${AUTO_ROOT_PATH}/src)

add_executable(${TARGET_NAME} ${SOURCE_ALL_FILES})