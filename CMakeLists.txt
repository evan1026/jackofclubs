cmake_minimum_required(VERSION 2.8)

project(jackofclubs)

set(SOURCE_DIR "src")
set(INCLUDE_DIR "include")
set(EXECUTABLE_NAME "jackofclubs")
set(DEBUG true)

if(DEBUG)
    set(CMAKE_CXX_FLAGS "-g -std=c++11 -Wall -Werror ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls ${CMAKE_CXX_FLAGS}")
    #set(CMAKE_CXX_FLAGS "-fsanitize=memory ${CMAKE_CXX_FLAGS}")
    #set(CMAKE_CXX_FLAGS "-fsanitize=address ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "-O3 -std=c++11 ${CMAKE_CXX_FLAGS}")
endif(DEBUG)

set(SOURCES "" CACHE STRING "Sources for project" FORCE)
get_filename_component(ROOT_DIR_ABS ${SOURCE_DIR} ABSOLUTE)
get_filename_component(ROOT_DIR ${ROOT_DIR_ABS} DIRECTORY)
add_subdirectory("${SOURCE_DIR}")

include_directories(${INCLUDE_DIR})

string(REPLACE "\n" " " SOURCES_WITHOUT_NL ${SOURCES})
set(SOURCES ${SOURCES_WITHOUT_NL} CACHE STRING "Sources for project" FORCE)
string(REPLACE " " ";" SOURCE_LIST ${SOURCES})
add_executable(${EXECUTABLE_NAME} ${SOURCE_LIST})

set(CMAKE_MODULE_PATH "/usr/local/share/SFML/cmake/Modules/;${CMAKE_MODULE_PATH}")

find_package(SFML 2 REQUIRED graphics window system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

find_package(OpenGL REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
