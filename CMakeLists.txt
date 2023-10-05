cmake_minimum_required(VERSION 3.12)
project(AutomatixClient)

# 设置 C++ 标准（根据实际需求进行调整）
set(CMAKE_CXX_STANDARD 11)

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# 添加可执行文件
add_executable(${PROJECT_NAME} main.cpp)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 链接库目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 链接所需的库文件
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/lib/nakama-sdk.lib)

# 复制 DLL 文件到构建目标目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PROJECT_SOURCE_DIR}/lib/nakama-sdk.dll>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
