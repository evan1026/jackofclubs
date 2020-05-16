file(RELATIVE_PATH CURR_DIR ${ROOT_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE("Adding ${CURR_DIR}")

set(SOURCES "${SOURCES} ${CURR_DIR}/BlockSelection.cpp ${CURR_DIR}/RenderEngine.cpp ${CURR_DIR}/Renderable.cpp ${CURR_DIR}/Shader.cpp ${CURR_DIR}/ShaderProgram.cpp" CACHE STRING "Sources for project" FORCE)
