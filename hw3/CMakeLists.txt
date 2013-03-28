cmake_minimum_required(VERSION 2.6)

project(render)

IF(WIN32)
    set(CMAKE_CXX_FLAGS  "-O2")
ELSE()
    set(CMAKE_CXX_FLAGS  "-g --std=c++0x -Wall")
ENDIF()

include_directories(. parsers glm)
#link_directories()

add_executable(render
    main.cpp
    Program.cpp
    Shader.cpp
    Util.cpp
    Mesh.cpp
    Scene.cpp
    Camera.cpp
    RenderedObject.cpp
    parsers/PlyParser.cpp
    parsers/ply.c
)

IF(WIN32)
    target_link_libraries(render glut glew32 GL)
ELSE()
    target_link_libraries(render glut GLEW GL)
ENDIF()


