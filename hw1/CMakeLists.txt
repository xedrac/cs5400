cmake_minimum_required(VERSION 2.6)

project(mountain)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
set(CMAKE_CXX_FLAGS  "-g -O2 --std=c++0x -Wall")

include_directories(glm)
#link_directories()

add_executable(mountain
    main.cpp
    Program.cpp
    Shader.cpp
    Mountain.cpp
    Util.cpp
)

target_link_libraries(mountain glut GLEW GL)

