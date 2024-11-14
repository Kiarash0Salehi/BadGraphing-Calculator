cmake_minimum_required (VERSION 3.6)

project (Graphing-Calculator)

add_executable (${PROJECT_NAME} WIN32 "src/graphic.c" "src/main.c" "src/thread.c" "src/vector.c" "src/window.c" "src/mathbase.c" "src/header/graphic.h" "src/header/bitmap.h" "src/header/WindowProc.h" "src/header/global.h" "src/header/thread.h" "src/header/vector.h" "src/header/window.h" "src/header/mathbase.h")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
