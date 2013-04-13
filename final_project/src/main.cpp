#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GameState.hpp"

using namespace std;

// Hack for GLUT callbacks to call C++ member functions
void onKey(unsigned char key, int x, int y) { GameState::getInstance().onKey(key, x, y);        }
void onSpecialKey(int key, int x, int y)    { GameState::getInstance().onSpecialKey(key, x, y); }


 
int main(int argc, char **argv)
{
    // Glut-related initialising functions
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH/*|GLUT_MULTISAMPLE*/);
    glutInitWindowSize(1680, 1050);
    glutCreateWindow("final project");

    // Extension wrangler initialising
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return EXIT_FAILURE;
    }

    try {  
        // Set the callbacks
        // Because GLUT is C (not c++), and we want the callbacks to be members of the
        // GameState class, we have to use thunks in order to hack it into submission
        glutKeyboardFunc(onKey);
        glutSpecialFunc(onSpecialKey);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        GameState &game = GameState::getInstance();
        game.init();
        game.gameLoop();

    } catch (exception &e) {
        cerr << endl << endl << e.what() << endl << endl;
    }

    return EXIT_SUCCESS;
}

