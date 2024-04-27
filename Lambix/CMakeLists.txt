cmake_minimum_required(VERSION 3.20)

project(Lambix LANGUAGES CXX)

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
file(GLOB_RECURSE LAMBIX_SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

# thirdParty
find_package(spdlog REQUIRED)

# 生成静态库
add_library(${PROJECT_NAME} ${LAMBIX_SOURCE_FILES})

# 预编译头
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/lbpch.h")

# 链接库
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        spdlog::spdlog
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        LB_PLATFORM_WINDOWS
    )
endif()