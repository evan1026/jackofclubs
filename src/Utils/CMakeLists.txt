file(RELATIVE_PATH CURR_DIR ${ROOT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE("Adding ${CURR_DIR}")

set(SOURCES "${SOURCES} ${CURR_DIR}/AABB.cpp
                        ${CURR_DIR}/ICollidable.cpp
                        ${CURR_DIR}/Font.cpp" CACHE STRING "Sources for project" FORCE)
