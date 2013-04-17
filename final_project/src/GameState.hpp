#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Program.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Spaceship.hpp"
#include "Projectile.hpp"
#include "SkyBox.hpp"
#include "RNG.hpp"
#include <vector>
#include <list>
#include <map>



class GameState
{
public:
    GameState();

    void init();
    void resetGame();
    void gameLoop();

    // Singleton instance of our gamestate
    static GameState &getInstance() { static GameState game; return game; }

public:
    void onKey(unsigned char key, int x, int y);
    void onKeyRelease(unsigned char key, int x, int y);
    void onSpecialKey(int key, int x, int y);
    void onMouseButton(int button, int state, int x, int y);
    void onMousePassiveMotion(int x, int y);

private:
    std::shared_ptr<Mesh> loadMesh(const std::string &name, const std::string &filename);
    void loadEnemyShips(std::shared_ptr<Mesh> mesh, int enemyrows, int enemycols, float speed, float scale, glm::vec3 rotate);
    void addEnemy(std::shared_ptr<Spaceship> enemy);
    void addPlayerProjectile(std::shared_ptr<Projectile> p);
	void updateObjectState();  // update all objects in scene
	void refreshTime();        // refresh time before starting a scene render (after models are added)
    void fireProjectile(glm::vec3 position, glm::vec3 direction, float scale, float speed, bool fromenemy);
    void createEffect(ParticleSystemType type, glm::vec3 position);

    enum MoveState {
        MoveStateLeft  = 0x0001,
        MoveStateRight = 0x0002,
        MoveStateUp    = 0x0004,
        MoveStateDown  = 0x0008,
    };

private:
    Scene _scene;
    Camera _camera;
    GLuint _program;
    GLuint _programParticles;

    RNG _rng;

    std::map<std::string, std::shared_ptr<Mesh>> _meshes;
    std::vector<std::shared_ptr<Spaceship>>      _enemyships;
    std::shared_ptr<Spaceship>                   _playership;
    std::vector<std::shared_ptr<ParticleSystem>> _particlesystems;
    std::list<std::shared_ptr<Projectile>>       _enemyprojectiles;
    std::list<std::shared_ptr<Projectile>>       _playerprojectiles;

    std::shared_ptr<SkyBox>                      _skybox;
    int _playermovestate;
	int _lastupdate;
    bool _freecamera;
    bool _running;
    bool _gameover;

    int _windowwidth;
    int _windowheight;
};

#endif

