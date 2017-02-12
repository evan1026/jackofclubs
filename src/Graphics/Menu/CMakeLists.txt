file(RELATIVE_PATH CURR_DIR ${ROOT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE("Adding ${CURR_DIR}")

set(SOURCES "${SOURCES} ${CURR_DIR}/Menu.cpp
                        ${CURR_DIR}/ColorSelectorMenu.cpp
                        ${CURR_DIR}/EscapeMenu.cpp" CACHE STRING "Sources for project" FORCE)
