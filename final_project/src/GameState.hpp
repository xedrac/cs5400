#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Program.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Spaceship.hpp"
#include <vector>
#include <map>



class GameState
{
public:
    void init();
    void gameLoop();

    // Singleton instance of our gamestate
    static GameState &getInstance() { static GameState game; return game; }

public:
    void onKey(unsigned char key, int, int);
    void onSpecialKey(int key, int, int);

private:
    std::shared_ptr<Mesh> loadMesh(const std::string &name, const std::string &filename);
    void loadEnemyShips(std::shared_ptr<Mesh> mesh, int enemyrows, int enemycols, float speed, float scale);
    void addEnemy(std::shared_ptr<Spaceship> enemy);
	void updateObjectState();  // update all objects in scene
	void refreshTime();        // refresh time before starting a scene render (after models are added)

    std::shared_ptr<cs5400::Program> getProgram() { return _program; }

private:
    Scene _scene;
    Camera _camera;

    std::map<std::string, std::shared_ptr<Mesh>> _meshes;
    std::shared_ptr<cs5400::Program> _program;
    std::vector<std::shared_ptr<Spaceship>> _enemyships;
    std::shared_ptr<Spaceship> _playership;
    std::vector< std::shared_ptr<ParticleSystem> > _particlesystems;

	int _lastupdate;
    bool _running;
};

#endif
