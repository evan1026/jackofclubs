file(RELATIVE_PATH CURR_DIR ${ROOT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE("Adding ${CURR_DIR}")

add_subdirectory("Components")
add_subdirectory("Menu")
add_subdirectory("Screen")
