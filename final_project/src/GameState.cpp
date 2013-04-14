#include <GL/glew.h>
#include <GL/freeglut.h>
#include "PlyParser.hpp"
#include "GameState.hpp"
#include "Program.hpp"
#include <assert.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;


void GameState::init()
{
    _program = cs5400::make_program(cs5400::make_vertexShader("shaders/vertex.glsl"),
                                    cs5400::make_fragmentShader("shaders/fragment.glsl"));

    Light light1(glm::vec3(0.0, 1.0, 1.5),  // position
                 glm::vec3(0.9, 1.0, 0.8),  // diffuse
                 glm::vec3(1.0, 1.0, 1.0)); // specular

    _scene.init();
    _scene.addLight(light1);
    _scene.setAmbientLight(glm::vec3(0.1, 0.1, 0.1));

    shared_ptr<Mesh> bunnymesh  = loadMesh("bunny",  "models/bunny.ply");
    //shared_ptr<Mesh> dragonmesh = loadMesh("dragon", "models/dragon.ply");

    int enemyrows = 5;
    int enemycols = 12;
    float enemyspeed = 0.0002;
    float enemyscale = 0.55;
    loadEnemyShips(bunnymesh, enemyrows, enemycols, enemyspeed, enemyscale);

    _playership = make_shared<Spaceship>(Spaceship(_program->getHandle(),
                                                   bunnymesh,
                                                   glm::vec3(0.0f, -0.7f, 0.0f ), // position
                                                   glm::vec3(0.4f,  0.4f, 0.4f),  // scale
                                                   glm::vec3(0.0f,  0.0f, 0.0f ), // rotation
                                                   -1,                            // direction
                                                   0.0001));                      // speed
    _playership->setId(_scene.getNewObjectId());
    _scene.insertObject(_playership);

    // test particle system
    std::shared_ptr<ParticleSystem> ps = make_shared<ParticleSystem>(ParticleSystem(10));
    _particlesystems.push_back(ps);
    _scene.insertParticleSystem(ps);

    refreshTime();

    _camera.setScene(&_scene);
    _camera.setPosition(glm::vec3(0.0f, 0.0f, 1.8f));
    _camera.render();

	_lastupdate = glutGet(GLUT_ELAPSED_TIME);
}


// This is our main event loop
void GameState::gameLoop()
{
    _running = true;

    while (_running) {
        glutMainLoopEvent();  // process any glut events that have queued up
        updateObjectState();  // move game objects as appropriate

        // Clear the background as black
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // render the scene
        _camera.render();

        // Display the newly rendered image to the screen
        glutSwapBuffers();
    }
}


void GameState::onKey(unsigned char key, int, int)
{
    // TODO:  Make a Camera class to handle viewing stuff
    switch(key) {
    case 'w':  _camera.moveY( 0.01);  break;
    case 's':  _camera.moveY(-0.01);  break;
    case 'a':  _camera.moveX(-0.01);  break;
    case 'd':  _camera.moveX( 0.01);  break;
    case 'q':  _camera.moveZ( 0.01);  break;
    case 'e':  _camera.moveZ(-0.01);  break;
    default:
        break;
    }

    glutPostRedisplay();
}


void GameState::onSpecialKey(int key, int, int)
{
    switch(key) {
        case GLUT_KEY_PAGE_UP:   _camera.roll(-1.5);  break;
        case GLUT_KEY_PAGE_DOWN: _camera.roll( 1.5);  break;
        case GLUT_KEY_LEFT:      _camera.yaw( 1.5);   break;
        case GLUT_KEY_RIGHT:     _camera.yaw(-1.5);   break;
        case GLUT_KEY_UP:        _camera.pitch( 1.5); break;
        case GLUT_KEY_DOWN:      _camera.pitch(-1.5); break;
        default:
            break;
    }

    glutPostRedisplay();
}

 
shared_ptr<Mesh> GameState::loadMesh(const string &name, const string &filename)
{
    shared_ptr<Mesh> mesh = parsePlyFile(filename);
    _meshes[name] = mesh;
    return mesh;
}


// update state of all objects
void GameState::updateObjectState()
{
	// get elapsed ms since last update
	int elapsedms = glutGet(GLUT_ELAPSED_TIME) - _lastupdate;
	bool keepDirection = true;

	// update all objects' state.  If one enemy changes direction,
    // make all enemies change direction (space invaders!)
    for (size_t i=0; i<_enemyships.size(); i++) {
        if (_enemyships[i]->update(elapsedms) == false) {
            keepDirection = false;
        }
    }

    if (!keepDirection) {
        for (size_t i=0; i<_enemyships.size(); i++)
            _enemyships[i]->changeDirection();
    }

    _playership->update(elapsedms);

	// check if playership collides with enemy ships
    //Material normalMat(...); 
    //Material collisionMat(...); 

    bool collision = false;
    for (size_t i=0; i<_enemyships.size(); i++) {
        if (!_playership->intersects(_enemyships[i]))
            continue;

        //_enemyships[i].setMaterial
        collision = true;
        break;
	}

	// for demo, if we have a collision we'll change light to red, otherwise the green
	if (collision) {
        //TODO:  player ship dies - cause explosion
        _scene.setAmbientLight(glm::vec3(1.0, 0.2, 0.2));
		//_lights[0].setDiffuse(glm::vec3(1.0, 0.2, 0.2));
	} else {
        _scene.setAmbientLight(glm::vec3(0.9, 1.0, 0.8));
		//_lights[0].setDiffuse(glm::vec3(0.9, 1.0, 0.8));
    }

	// set new last update time
	_lastupdate = glutGet(GLUT_ELAPSED_TIME);
} 

// refreshes time before starting a scene render after models have been loaded
void GameState::refreshTime()
{
	_lastupdate = glutGet(GLUT_ELAPSED_TIME);
}


void GameState::addEnemy(shared_ptr<Spaceship> enemy)
{
    if (enemy->getId() == INVALID_OBJECTID)
        enemy->setId(_scene.getNewObjectId());

    _enemyships.push_back(enemy);
    _scene.insertObject(enemy);
}



void GameState::loadEnemyShips(shared_ptr<Mesh> mesh, int enemyrows, int enemycols, float speed, float scale)
{
    shared_ptr<Spaceship> enemy;

    // Limit the max rows/cols
    if (enemycols > 20) enemycols = 20;
    if (enemyrows > 10) enemyrows = 10;

    // half of the columns minus the center column
    int halfcols = (int)(enemycols / 2);
    int halfrows = (int)(enemyrows / 2);

    float screenwidth = 1.8;  // (-1.0, 1.0)
    float halfwidth = screenwidth / 2.0f;
    float xgap = screenwidth / enemycols;

    for (int row=0; row<enemyrows; row++) {
        float ypos = (row - halfrows) * 0.1 + 0.4;
        for (int col=0; col<enemycols; col++) {
            float xpos = col*xgap - halfwidth;
            //float xpos = (col - halfcols) * 0.1;
            enemy = make_shared<Spaceship>(Spaceship(_program->getHandle(),
                                                     mesh,
                                                     glm::vec3(xpos, ypos, 0.0f), // position
                                                     glm::vec3(scale),            // scale
                                                     glm::vec3(0.0f, 0.0f, 0.0f), // rotation
                                                     1,                           // direction
                                                     speed));                     // speed
            addEnemy(enemy);


        }
    }
}



