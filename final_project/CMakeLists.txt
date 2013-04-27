cmake_minimum_required(VERSION 2.6)

project(spaceinvaders)

#set (CMAKE_PREFIX_PATH /usr/lib/x86_64-linux-gnu/)

find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS  "-g --std=c++0x -Wall")

#include(clang.cmake)

include_directories(
    ${GLUT_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    src
    externals)

#link_directories(/usr/lib/x86-64/-linux-gnu/)

add_executable(spaceinvaders
    src/Program.cpp
    src/BoundingBox.cpp
    src/Shader.cpp
    src/Util.cpp
    src/Mesh.cpp
    src/Scene.cpp
    src/Camera.cpp
    src/RenderedObject.cpp
    src/PlyParser.cpp
    src/ObjParser.cpp
    src/Texture.cpp
    src/GameState.cpp
    src/Spaceship.cpp
    src/Particle.cpp
    src/ParticleSystem.cpp
    src/Projectile.cpp
    src/FlatSurface.cpp
    src/SkyBox.cpp
    src/main.cpp
    src/RNG.cpp
    externals/parsers/ply/ply.c
)


target_link_libraries(spaceinvaders
    ${GLUT_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES})

file(COPY shaders  DESTINATION ${CMAKE_BINARY_DIR})
file(COPY models   DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})

