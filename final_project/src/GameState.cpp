#include <GL/glew.h>
#include <GL/freeglut.h>
#include "PlyParser.hpp"
#include "ObjParser.hpp"
#include "GameState.hpp"
#include "Program.hpp"
#include <assert.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

GameState::GameState()
    : _playermovestate(0)
{
    _program = cs5400::make_program(cs5400::make_vertexShader("shaders/vertex.glsl"),
                                    cs5400::make_fragmentShader("shaders/fragment.glsl"));

    Light light1(glm::vec3(0.0, 1.0, 1.5),  // position
                 glm::vec3(0.9, 1.0, 0.8),  // diffuse
                 glm::vec3(1.0, 1.0, 1.0)); // specular

    _scene.init();
    _scene.addLight(light1);
    _scene.setAmbientLight(glm::vec3(0.1, 0.1, 0.1));

    _camera.setScene(&_scene);
    _camera.setPosition(glm::vec3(0.0f, 0.0f, 1.8f));

    _skybox = make_shared<SkyBox>(SkyBox(_program->getHandle(),
                                         "textures/grimmnight-top.png",
                                         "textures/grimmnight-bottom.png",
                                         "textures/grimmnight-left.png",
                                         "textures/grimmnight-right.png",
                                         "textures/grimmnight-front.png",
                                         "textures/grimmnight-back.png"));
    _skybox->addToScene(_scene);

    shared_ptr<Mesh> bunnymesh        = loadMesh("bunny",  "models/bunny.ply");
    shared_ptr<Mesh> spacefrigatemesh = loadMesh("space_frigate",  "models/space_frigate.obj");
    shared_ptr<Mesh> spaceshipmesh    = loadMesh("spaceship",  "models/spaceship.obj");
}


void GameState::init()
{

    int enemyrows = 4;
    int enemycols = 8;
    float enemyspeed = 0.0002;
    float enemyscale = 0.003;
    glm::vec3 enemyrotate = glm::vec3(90.0, 0.0, 90.0);
    loadEnemyShips(_meshes["space_frigate"], enemyrows, enemycols, enemyspeed, enemyscale, enemyrotate);

    _playership = make_shared<Spaceship>(Spaceship(_program->getHandle(),
                                                   _meshes["space_frigate"],
                                                   glm::vec3(0.0f, -0.6f, 0.0f ),         // position
                                                   glm::vec3(0.004f, 0.004f, 0.004f),  // scale
                                                   glm::vec3(90.f, 0.f, -90.f),            // rotation
                                                   -1,                                    // direction
                                                   0.0004));                              // speed
    Material m;
    m.setAmbient(glm::vec3(0.2, 0.2, 0.2));
    m.setDiffuse(glm::vec3(1.0, 1.0, 0.5));
    m.setSpecular(glm::vec3(0.9, 0.9, 0.9));
    m.setShininess(100.0);

    _playership->setMaterial(m);
    _playership->setId(_scene.getNewObjectId());
    _scene.insertObject(_playership);

    // test particle system
    std::shared_ptr<ParticleSystem> ps = make_shared<ParticleSystem>(ParticleSystem(10));
    _particlesystems.push_back(ps);
    _scene.insertParticleSystem(ps);

    refreshTime();

    _camera.render();

	_lastupdate = glutGet(GLUT_ELAPSED_TIME);
    _gameover = false;
}


// This is our main event loop
void GameState::gameLoop()
{
    _running = true;

    while (_running) {
        glutMainLoopEvent();  // process any glut events that have queued up
        updateObjectState();  // move game objects as appropriate

        if (_gameover) {
            cout << "Game Over\n";
            return;
        }

        // Clear the background as black
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // render the scene
        _camera.render();

        // Display the newly rendered image to the screen
        glutSwapBuffers();
    }
}

void GameState::onKeyRelease(unsigned char key, int, int)
{
    switch(key) {
    case 'w':  // fall through
    case ',': _playermovestate &= ~MoveStateUp;    break;

    case 'a': _playermovestate &= ~MoveStateLeft;  break;

    case 's':  // fall through
    case 'o': _playermovestate &= ~MoveStateDown;  break;

    case 'd':  // fall through
    case 'e': _playermovestate &= ~MoveStateRight; break;
    };
}


void GameState::onKey(unsigned char key, int, int)
{
    switch(key) {
    case 'w':  // fall through
    case ',': _playermovestate |= MoveStateUp;    break;

    case 'a': _playermovestate |= MoveStateLeft;  break;
    case 's':  // fall through

    case 'o': _playermovestate |= MoveStateDown;  break;
    case 'd':  // fall through

    case 'e': _playermovestate |= MoveStateRight; break;
    case ' ':
        break;
    default:
        break;
    }

    glutPostRedisplay();
}


void GameState::onSpecialKey(int key, int, int)
{
    switch(key) {
        //case GLUT_KEY_PAGE_UP:   _camera.roll (-1.5); break;
        //case GLUT_KEY_PAGE_DOWN: _camera.roll ( 1.5); break;
        //case GLUT_KEY_LEFT:      _camera.yaw  ( 1.5); break;
        //case GLUT_KEY_RIGHT:     _camera.yaw  (-1.5); break;
        //case GLUT_KEY_UP:        _camera.pitch( 1.5); break;
        //case GLUT_KEY_DOWN:      _camera.pitch(-1.5); break;
        default:
            break;
    }

    glutPostRedisplay();
}


void GameState::fireProjectile(glm::vec3 position, glm::vec3 direction, float scale, float speed, bool fromenemy)
{
    shared_ptr<Projectile> p = make_shared<Projectile>(Projectile(_program->getHandle(),
                                                                  _meshes["bunny"],
                                                                  position,
                                                                  glm::vec3(scale, scale, scale),
                                                                  glm::vec3(1.0f),           
                                                                  direction,
                                                                  speed));
    if (fromenemy) _enemyprojectiles.push_back(p);
    else           _playerprojectiles.push_back(p);

    p->setId(_scene.getNewObjectId());
    _scene.insertObject(p);
}


void GameState::onMouseButton(int button, int state, int, int)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
            fireProjectile(_playership->getPosition(), glm::vec3(0.0f, 1.0f, 0.0f), 0.1, 0.001, false);
        break;

    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            fireProjectile(_playership->getPosition(), glm::vec3(0.0f, 1.0f, 0.0f), 0.5, 0.001, false);
        break;

    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN)
            fireProjectile(_playership->getPosition(), glm::vec3(0.0f, 1.0f, 0.0f), 1.5, 0.001, false);
        break;

    case 3:  _camera.moveZ(0.02);  break;
    case 4:  _camera.moveZ(-0.02); break;
    }
}

 
shared_ptr<Mesh> GameState::loadMesh(const string &name, const string &filename)
{
    shared_ptr<Mesh> mesh;

    if (filename.find(".ply") != string::npos) {
        mesh = parsePlyFile(filename);
    } else if (filename.find(".obj") != string::npos) {
        mesh = ObjParser::parse(filename);
    }

    _meshes[name] = mesh;
    return mesh;
}


// update state of all objects
void GameState::updateObjectState()
{
	// get elapsed ms since last update
	int elapsedms = glutGet(GLUT_ELAPSED_TIME) - _lastupdate;

    if (_playermovestate & MoveStateLeft)  _playership->translate(glm::vec3(-0.01f, 0.0f,  0.0f));
    if (_playermovestate & MoveStateRight) _playership->translate(glm::vec3( 0.01f, 0.0f,  0.0f));
    if (_playermovestate & MoveStateUp)    _playership->translate(glm::vec3( 0.0f,  0.01f, 0.0f));
    if (_playermovestate & MoveStateDown)  _playership->translate(glm::vec3( 0.0f, -0.01f, 0.0f));


	// update all objects' state.  If one enemy changes direction,
    // make all enemies change direction (space invaders!)
	bool keepDirection = true;
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

    // Update position of projectiles and check for collisions with player
    auto epp = _enemyprojectiles.begin();
    while (epp != _enemyprojectiles.end()) {
        shared_ptr<Projectile> projectile = *epp;

        projectile->update(elapsedms);

        if (!projectile->intersects(_playership)) {
            ++epp;
            continue;
        }

        // GAME OVER
        _scene.removeObject(_playership);
        epp = _enemyprojectiles.erase(epp);
        _gameover = true;
        break;
    }

    
    // Check if player shot down any enemies, also clean up
    // _playerprojectiles list if previously destroyed projectiles still exist
    auto ppitr = _playerprojectiles.begin();
    while (ppitr != _playerprojectiles.end()) {
        shared_ptr<Projectile> projectile = *ppitr;

        // Remove projectiles that have flown off the screen
        if (projectile->update(elapsedms)) {
            _scene.removeObject(projectile);
            ppitr = _playerprojectiles.erase(ppitr);
            continue;
        }

        bool impact = false;
        auto eitr = _enemyships.begin();

        while (eitr != _enemyships.end()) {
            shared_ptr<Spaceship> enemy = *eitr;
            if (!enemy || !projectile->intersects(enemy)) {
                ++eitr;
                continue;
            }

            // BOOM!  Destroy enemy ship & projectile
            _scene.removeObject(enemy);
            _scene.removeObject(projectile);
            
            eitr = _enemyships.erase(eitr);
            ppitr = _playerprojectiles.erase(ppitr);
            impact = true;
            break;
        }

        if (!impact)
            ++ppitr;
    }


    // check if player ship collides with enemy ship
    // bool collision = false;
    Box3D box = _playership->getBoundingBox().getBounds();
    //cout << "Player BoundingBox: (" << box.x0 << "," << box.x1 << ") "
    //                         << "(" << box.y0 << "," << box.y1 << ") "
    //                         << "(" << box.z0 << "," << box.z1 << ")\n";
    for (size_t i=0; i<_enemyships.size(); i++) {
        box = _enemyships[i]->getBoundingBox().getBounds();
        if (_playership->intersects(_enemyships[i])) {
            //cout << "Enemy[" << i << "] BoundingBox: " << "(" << box.x0 << "," << box.x1 << ") "
            //                                           << "(" << box.y0 << "," << box.y1 << ") "
            //                                           << "(" << box.z0 << "," << box.z1 << ")\n";
            //cout << "player intersected enemy " << i << endl;
            Material m;
            m.setAmbient(glm::vec3(0.3, 0.2, 0.2));
            m.setDiffuse(glm::vec3(0.8, 0.3, 0.3));
            m.setSpecular(glm::vec3(0.8, 0.8, 0.8));
            m.setShininess(100.0);
            _enemyships[i]->setMaterial(m);
            _enemyships[i]->rotate(glm::vec3(0, 0, 0), 2.0f);
        }

        //collision = true;
        //break;
	}

#if 0
	// for demo, if we have a collision we'll change light to red, otherwise the green
	if (collision) {
        //TODO:  player ship dies - cause explosion
        _scene.setAmbientLight(glm::vec3(1.0, 0.2, 0.2));
		//_lights[0].setDiffuse(glm::vec3(1.0, 0.2, 0.2));
	} else {
        _scene.setAmbientLight(glm::vec3(0.9, 1.0, 0.8));
		//_lights[0].setDiffuse(glm::vec3(0.9, 1.0, 0.8));
    }
#endif

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



void GameState::loadEnemyShips(shared_ptr<Mesh> mesh, int enemyrows, int enemycols, float speed, float scale, glm::vec3 rot)
{
    shared_ptr<Spaceship> enemy;

    // Limit the max rows/cols
    if (enemycols > 20) enemycols = 20;
    if (enemyrows > 10) enemyrows = 10;

    int halfrows      = (int)(enemyrows / 2);
    float screenwidth = 1.8;  // (-1.0, 1.0)
    float halfwidth   = screenwidth / 2.0f;
    float xgap        = screenwidth / enemycols;

    for (int row=0; row<enemyrows; row++) {
        float ypos = (row - halfrows) * 0.1 + 0.4;
        for (int col=0; col<enemycols; col++) {
            float xpos = col*xgap - halfwidth;
            enemy = make_shared<Spaceship>(Spaceship(_program->getHandle(),
                                                     mesh,
                                                     glm::vec3(xpos, ypos, 0.0f), // position
                                                     glm::vec3(scale),            // scale
                                                     glm::vec3(rot.x, rot.y, rot.z), // rotation
                                                     1,                           // direction
                                                     speed));                     // speed
            addEnemy(enemy);
        }
    }
}



