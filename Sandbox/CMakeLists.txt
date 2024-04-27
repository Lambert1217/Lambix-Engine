cmake_minimum_required(VERSION 3.20)

project(Sandbox LANGUAGES CXX)

# 设置c++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_PATH}/bin/${CMAKE_BUILD_TYPE})  # 动态库
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_PATH}/bin/${CMAKE_BUILD_TYPE})  # 可执行程序

# 包含路径
include_directories(
    SYSTEM ${ROOT_PATH}/Lambix/src
)

# 代码文件
file(GLOB_RECURSE SANDBOX_SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SANDBOX_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Lambix
)