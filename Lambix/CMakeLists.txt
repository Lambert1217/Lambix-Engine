cmake_minimum_required(VERSION 3.20)

project(Lambix LANGUAGES CXX)

# 设置c++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_PATH}/bin/${CMAKE_BUILD_TYPE})  # 动态库
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_PATH}/bin/${CMAKE_BUILD_TYPE})  # 可执行程序

# 代码文件
file(GLOB_RECURSE LAMBIX_SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

# thirdParty
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
add_subdirectory(thirdParty/glad)
# imgui
file(GLOB IMGUI_SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/thirdParty/imgui/*.cpp")
add_library(imgui ${IMGUI_SOURCE_FILES})
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/imgui)

# 生成静态库
add_library(${PROJECT_NAME} ${LAMBIX_SOURCE_FILES})

# 预编译头
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/lbpch.h")

# 头文件路径
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/glm
)

# 链接库
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        spdlog::spdlog
        imgui
        PRIVATE
        glfw3
        glad
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME}
        PUBLIC
        LB_PLATFORM_WINDOWS
        PRIVATE
        LB_ENABLE_ASSERTS
        GLFW_INCLUDE_NONE
        _CRT_SECURE_NO_WARNINGS
    )
endif()