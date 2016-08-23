file(RELATIVE_PATH CURR_DIR ${ROOT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE("Adding ${CURR_DIR}")

set(SOURCES "${SOURCES} ${CURR_DIR}/main.cpp
                        ${CURR_DIR}/Player.cpp
                        ${CURR_DIR}/Game.cpp" CACHE STRING "Sources for project" FORCE)

add_subdirectory("Exception")
add_subdirectory("Rendering")
add_subdirectory("Screen")
add_subdirectory("Utils")
add_subdirectory("World")
