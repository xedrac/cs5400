#include <GL/glew.h>
#include <GL/glut.h>
#include "Shader.hpp"
#include "Program.hpp"
#include "Mountain.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "defs.h"

#include <iostream>
#include <vector>

using namespace std;

// globals

Scene scene;
Camera camera;


void onDisplay()
{
    // Clear the background as black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // render the scene
    camera.render();

    // Display the newly rendered image to the screen
    glutSwapBuffers();
}


void onKey(unsigned char key, int, int)
{
    // TODO:  Make a Camera class to handle viewing stuff
    switch(key) {
    case 'w':  camera.moveY( 0.01);  break;
    case 's':  camera.moveY(-0.01);  break;
    case 'a':  camera.moveX(-0.01);  break;
    case 'd':  camera.moveX( 0.01);  break;
    case 'q':  camera.moveZ( 0.01);  break;
    case 'e':  camera.moveZ(-0.01);  break;
    default:
        break;
    }

    glutPostRedisplay();
}


void onSpecialKey(int key, int, int)
{
    switch(key) {
        case GLUT_KEY_PAGE_UP:   camera.roll(-1.0);  break;
        case GLUT_KEY_PAGE_DOWN: camera.roll( 1.0);  break;
        case GLUT_KEY_LEFT:      camera.yaw( 1.0);   break;
        case GLUT_KEY_RIGHT:     camera.yaw(-1.0);   break;
        case GLUT_KEY_UP:        camera.pitch( 1.0); break;
        case GLUT_KEY_DOWN:      camera.pitch(-1.0); break;
        default:
            break;
    }

    glutPostRedisplay();
}

 
void onIdle(void)
{
  
}
 
 
int main(int argc, char **argv)
{
    if (argc < 2) {
        cerr << "\n\n\tSyntax: " << argv[0] << " <plyfile>\n\n";
        return EXIT_FAILURE;
    }

    string filepath = argv[1];

    // Glut-related initialising functions
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
    glutInitWindowSize(1680, 1050);
    glutCreateWindow("hw2");

    cout << "\n\n\t***********************************\n";
    cout <<     "\t* See README file for camera keys *\n";
    cout <<     "\t***********************************\n\n\n";

    // Extension wrangler initialising
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return EXIT_FAILURE;
    }

    try {  
        glutDisplayFunc(onDisplay);
        glutKeyboardFunc(onKey);
        glutSpecialFunc(onSpecialKey);
        glutIdleFunc(onIdle);

        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        
        Light light1(glm::vec3(0.0f, 1.7f, 0.5f), // Position
                     glm::vec3(1.0f, 1.0f, 1.0f),  // ambient
                     glm::vec3(1.0f, 1.0f, 1.0f),  // diffuse
                     glm::vec3(1.0f, 1.0f, 1.0f)); // specular
        
        scene.init();
        //scene.setAmbientLight(glm::vec3(0.1, 0.1, 0.1));
        scene.addLight(light1);
        scene.loadObjectFromPly(filepath);

        camera.setScene(&scene);
        camera.render();
        glutMainLoop();

    } catch (exception &e) {
        cerr << endl << endl << e.what() << endl << endl;
    }

    return EXIT_SUCCESS;
}
