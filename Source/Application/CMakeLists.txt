# Source/Application/CMakeLists.txt

# If your top-level already included HandleQt.cmake, Qt is found.
# Re-finding here is harmless and makes this dir self-contained:
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Gather sources (or list explicitly)
file(GLOB_RECURSE APP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(Editor
        MANUAL_FINALIZATION
        ${APP_SOURCES}
    )
else()
    add_executable(Editor ${APP_SOURCES})
endif()

# Add the root of your project headers so <Application/Core/Core.h> works
target_include_directories(Editor PRIVATE
    "${CMAKE_SOURCE_DIR}/Source"
)

target_link_libraries(Editor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Editor)
endif()