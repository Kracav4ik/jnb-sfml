cmake_minimum_required(VERSION 3.5)
project(jnb_sfml)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(EXECUTABLE_NAME "jnb_sfml")
set(SOURCE_FILES src/main.cpp src/level.cpp src/level.h src/rabbit.cpp src/rabbit.h src/utils.cpp src/utils.h src/params.cpp src/params.h src/debug.cpp src/debug.h src/render.cpp src/render.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/SFML-2.3.2")
set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules" ${CMAKE_MODULE_PATH})

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 REQUIRED graphics window system)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()
