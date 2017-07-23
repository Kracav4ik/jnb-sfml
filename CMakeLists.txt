cmake_minimum_required(VERSION 3.5)
project(jnb_sfml)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

message("qtdir is set to '$ENV{QTDIR}' (must be 'c:/Qt/5.7/mingw53_32/' or similar)")
set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(GAME_NAME "jnb_sfml")
set(EDITOR_NAME "jnb_edit")
set(COMMON_SOURCES src/level.cpp src/level.h src/utils.cpp src/utils.h src/params.cpp src/params.h src/debug.cpp src/debug.h src/render.cpp src/render.h src/texture_manager.cpp src/texture_manager.h src/animation.cpp src/animation.h src/file_path.cpp src/file_path.h src/frame_animation.cpp src/frame_animation.h)
set(GAME_SOURCES src/main.cpp src/rabbit.cpp src/rabbit.h ${COMMON_SOURCES} src/hitInfo.h)
set(EDITOR_SOURCES src/editor_main.cpp src/editor_window.h src/editor_window.cpp src/editor.ui ${COMMON_SOURCES} src/background_grid.cpp src/background_grid.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(${GAME_NAME} ${GAME_SOURCES})
add_executable(${EDITOR_NAME} ${EDITOR_SOURCES})

set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML-2.3.2")
set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules" ${CMAKE_MODULE_PATH})

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 REQUIRED graphics window system)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${GAME_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
    target_link_libraries(${EDITOR_NAME} Qt5::Widgets Qt5::Core ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()
