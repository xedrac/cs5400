cmake_minimum_required(VERSION 2.6)

project(render)

find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS  "-g --std=c++0x -Wall")

include_directories(
    ${GLEW_INCLUDE_PATH}
    src
    externals/parsers
    externals/glm
    externals/lodepng)

#link_directories()

add_executable(render
    src/main.cpp
    src/Program.cpp
    src/Shader.cpp
    src/Util.cpp
    src/Mesh.cpp
    src/Scene.cpp
    src/Camera.cpp
    src/RenderedObject.cpp
    src/PlyParser.cpp
    externals/parsers/ply.c
    externals/lodepng/lodepng.cpp
)

target_link_libraries(render ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})

