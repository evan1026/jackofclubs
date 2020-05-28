cmake_minimum_required(VERSION 2.8)

project(jackofclubs)

############
# SETTINGS #
############
set(SOURCE_DIR "src")
set(INCLUDE_DIR "include")
set(RESOURCES_DIR "resources")
set(LIB_DIR "lib")
set(EXECUTABLE_NAME "jackofclubs")
set(DEBUG true)

set(OpenGL_GL_PREFERENCE GLVND)

if(DEBUG)
    set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
    if(NOT WIN32)
        set(CMAKE_CXX_FLAGS "-g -O0 -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wall -Werror ${CMAKE_CXX_FLAGS}")
    endif()
    #set(CMAKE_CXX_FLAGS "-fsanitize=memory ${CMAKE_CXX_FLAGS}")
    #set(CMAKE_CXX_FLAGS "-fsanitize=address ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "-O3 -std=c++17 ${CMAKE_CXX_FLAGS}")
endif(DEBUG)

if(WIN32)
    #Set this to where you built SFML
    #Not needed on linux since running make install will put everything where it needs to be
    set(SFML_ROOT "C:/Users/evana/AppData/Local/CMakeBuild/0d32c7ec-8872-8434-a751-0947328ef528/build/install")
    set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "/usr/local/share/SFML/cmake/Modules/;${CMAKE_MODULE_PATH}")

##################
# GATHER SOURCES #
##################
set(SOURCES "" CACHE STRING "Sources for project" FORCE)
get_filename_component(ROOT_DIR_ABS ${SOURCE_DIR} ABSOLUTE)
get_filename_component(ROOT_DIR ${ROOT_DIR_ABS} DIRECTORY)
add_subdirectory("${SOURCE_DIR}")

include_directories(${INCLUDE_DIR})

string(REPLACE "\n" " " SOURCES_WITHOUT_NL ${SOURCES})
set(SOURCES ${SOURCES_WITHOUT_NL} CACHE STRING "Sources for project" FORCE)
string(REPLACE " " ";" SOURCE_LIST ${SOURCES})
add_executable(${EXECUTABLE_NAME} ${SOURCE_LIST})


#################
# ADD LIBRARIES #
#################
add_subdirectory(${LIB_DIR})
include_directories(${LIB_INCLUDES})
target_link_libraries(${EXECUTABLE_NAME} ${LIB_LIBRARIES})

find_package(SFML 2 REQUIRED graphics window system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

find_package(OpenGL REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} imagehlp.lib)
endif()


##################
# COPY RESOURCES #
##################
set(EXECUTABLE_LOCATION $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
add_custom_target(copy_resources ALL)
file(GLOB_RECURSE RESOURCE_FILES ${CMAKE_SOURCE_DIR}/${RESOURCES_DIR}/*)
add_custom_command(TARGET copy_resources PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    make_directory ${EXECUTABLE_LOCATION}/resources)
foreach(RESOURCE_FILE ${RESOURCE_FILES})
    add_custom_command(TARGET copy_resources PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E
        copy ${RESOURCE_FILE} ${EXECUTABLE_LOCATION}/resources/)
endforeach()

if(WIN32)
    # TODO custom targets for windows but eh right now I can't be bothered
    string(REGEX MATCH "[^;]*" FIRST_LIBRARY "${SFML_LIBRARIES}")
    string(REGEX MATCH "(.*/)*" LIBS_DIR "${FIRST_LIBRARY}")

    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${LIBS_DIR}/../bin/ $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/)

    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "C:/Program Files (x86)/Microsoft Visual Studio/2017/Professional/VC/Redist/MSVC/14.10.24728/" $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/)

endif()
