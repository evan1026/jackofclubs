cmake_minimum_required(VERSION 2.8)

project(jackofclubs)

set(SOURCE_DIR "src")
set(INCLUDE_DIR "src")
set(RESOURCES_DIR "resources")
set(EXECUTABLE_NAME "jackofclubs")
set(DEBUG true)

if(DEBUG)
    set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    if(NOT WIN32)
        set(CMAKE_CXX_FLAGS "-g -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wall -Werror ${CMAKE_CXX_FLAGS}")
    endif()
    #set(CMAKE_CXX_FLAGS "-fsanitize=memory ${CMAKE_CXX_FLAGS}")
    #set(CMAKE_CXX_FLAGS "-fsanitize=address ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "-O3 -std=c++11 ${CMAKE_CXX_FLAGS}")
endif(DEBUG)

if(WIN32)
    #Set this to where you built SFML
    #Not needed on linux since running make install will put everything where it needs to be
    set(SFML_ROOT "C:/Users/evana/AppData/Local/CMakeBuild/0d32c7ec-8872-8434-a751-0947328ef528/build/install")
    set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules")
endif()

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

if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} imagehlp.lib)
endif()

add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/${RESOURCES_DIR} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/resources)

if(WIN32)
    string(REGEX MATCH "[^;]*" FIRST_LIBRARY "${SFML_LIBRARIES}")
    string(REGEX MATCH "(.*/)*" LIBS_DIR "${FIRST_LIBRARY}")

    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${LIBS_DIR}/../bin/ $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/)

    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "C:/Program Files (x86)/Microsoft Visual Studio/2017/Professional/VC/Redist/MSVC/14.10.24728/" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/)

endif()