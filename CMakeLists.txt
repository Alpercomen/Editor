cmake_minimum_required(VERSION 3.20)
project(Editor LANGUAGES CXX)

# Nice to set once at top level (redundant but harmless):
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(SetupConfig)    # your file
include(HandleQt)       # <-- after project()

add_subdirectory(External)
add_subdirectory(Source/Application)

target_compile_definitions(Editor PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")