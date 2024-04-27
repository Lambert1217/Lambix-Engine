cmake_minimum_required(VERSION 3.20)

project(Lambix-Engine)

# 设置输出路径
set(ROOT_PATH ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Root path of the project")

# 子项目
add_subdirectory(${ROOT_PATH}/Lambix)   # 引擎
add_subdirectory(${ROOT_PATH}/Sandbox)  # Sandbox