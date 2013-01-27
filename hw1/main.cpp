#include <iostream>
//#include <functional>
//#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Shader.hpp"
#include "Program.hpp"
#include "Mountain.hpp"
#include "defs.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

// globals
shared_ptr<cs5400::Program> program;
GLuint vertexbuffer;
GLuint normalbuffer;
GLuint mvp_id;
GLuint view_id;
GLuint model_id;
GLuint light_id;

Mountain mountain;



void onDisplay()
{
    // Clear the background as black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Tell OpenGL which program to use
    glUseProgram(program->getHandle());

    // calculate the M V & P matricies
    glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 view       = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -4.0f));
    glm::mat4 model      = glm::mat4(1.0f);   // identity
    glm::mat4 mvp        = projection * view * model;
    glm::vec3 light      = glm::vec3(-1.0f, 1.0f, 1.0f);

    // pass our transformation matricies to the shaders
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));

    // tell the shaders where the light is
    glUniform3f(light_id, light.x, light.y, light.z);

    // Pass the vertex locations to the shader
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Pass the vertex normals to the shader
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, NULL);

    // Render the triangles
    const vector<Triangle> &triangles = mountain.getTriangles();
    glDrawArrays(GL_TRIANGLES, 0, triangles.size()*3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Display the newly rendered image to the screen
    glutSwapBuffers();
}
 
 
int main(int argc, char **argv)
{
    // Glut-related initialising functions
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("3D Mountain");

    // Extension wrangler initialising
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return EXIT_FAILURE;
    }

    // When all init functions runs without errors,
    // the program can initialise the resources
    try {  
        program = cs5400::make_program(cs5400::make_vertexShader("shaders/vertex.glsl"),
                                       cs5400::make_fragmentShader("shaders/fragment.glsl"));
        glutDisplayFunc(onDisplay);

        mvp_id   = glGetUniformLocation(program->getHandle(), "MVP");
        view_id  = glGetUniformLocation(program->getHandle(), "V");
        model_id = glGetUniformLocation(program->getHandle(), "M");
        light_id = glGetUniformLocation(program->getHandle(), "lightpos_world"); 

        mountain.generate(Point( 0.0,  1.0,  0.0),
                          Point(-1.0, -1.0,  1.0),
                          Point( 1.0, -1.0,  1.0),
                          Point( 0.0, -1.0, -1.0),
                          10);

        const vector<Triangle> &triangles = mountain.getTriangles();
        const vector<glm::vec3> &vertexnormals = mountain.getVertexNormals();

        // Bind the vertex locations to a GPU buffer
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, triangles.size()*sizeof(Triangle), &triangles[0], GL_STATIC_DRAW);

        // Bind the vertex normals to a GPU buffer
        glGenBuffers(1, &normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, vertexnormals.size()*sizeof(vertexnormals[0]), &vertexnormals[0], GL_STATIC_DRAW);

        glutMainLoop();

    } catch (exception &e) {
        cerr << e.what();
    }

    return EXIT_SUCCESS;
}

