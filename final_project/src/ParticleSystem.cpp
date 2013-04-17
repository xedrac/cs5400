#include "ParticleSystem.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

ParticleSystem::ParticleSystem(GLuint program, ParticleSystemType type, glm::vec3 startposition)
{
    _delay = 0; // default

    switch (type) {
        case ParticleSystemType::Explosion:
            createExplosion(startposition);
            break;
        case ParticleSystemType::PlayerExhaust:
            createPlayerExhaust(startposition);
            break;
        case ParticleSystemType::EnemyExhaust:
            createEnemyExhaust(startposition);
            break;
        case ParticleSystemType::Death:
            createDeath(startposition);
            break;
        case ParticleSystemType::DeathFollowup:
            createDeathFollowup(startposition);
            break;
        default:
            break;
    }

    _particleprogram = program;

    glGenBuffers(1, &_glvertexbuffer);
    glGenBuffers(1, &_glcolorbuffer);

    _glvertexattrib   = glGetAttribLocation (program, "vPosition");
    _glcolorattrib    = glGetAttribLocation (program, "vColor");

    _totalelapsed = 0;
    _visible = true;
    _type = type;
}

// Render the system
void ParticleSystem::render(GLuint modelmatrixid, 
                            GLuint viewmatrixid, 
                            GLuint projectionmatrixid, 
                            const glm::mat4 &viewmatrix, 
                            const glm::mat4 &projectionmatrix)
{
    // Don't render if it's been flagged as not visible
    if (!_visible)
        return;

    glUseProgram(_particleprogram);

    std::vector<glm::vec4> points;
    std::vector<glm::vec4> colors;

    for (int i = 0; i < _size; i++)
    {
        points.push_back(glm::vec4(_particles[i].getPosition(), 1.0));
        colors.push_back(_particles[i].getColor());
    }

    glPointSize(_particlesize);

    glm::mat4 model = glm::mat4(1.0);
    glUniformMatrix4fv(modelmatrixid, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewmatrixid, 1, GL_FALSE, glm::value_ptr(viewmatrix));
    glUniformMatrix4fv(projectionmatrixid, 1, GL_FALSE, glm::value_ptr(projectionmatrix));

    glEnableVertexAttribArray(_glvertexattrib);
    glBindBuffer(GL_ARRAY_BUFFER, _glvertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, _size*sizeof(glm::vec4), points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(_glvertexattrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(_glcolorattrib);
    glBindBuffer(GL_ARRAY_BUFFER, _glcolorbuffer);
    glBufferData(GL_ARRAY_BUFFER, _size*sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(_glcolorattrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, _size);

    glDisableVertexAttribArray(_glvertexattrib);
    glDisableVertexAttribArray(_glcolorattrib);
}

// Returns false if system has exceeded runtime
bool ParticleSystem::update(int elapsedms)
{
    // Don't update if it's been flagged as not visible
    if (!_visible)
        return true;

    if (_delay > 0) {
        _delay -= elapsedms;
        return true;
    }

    float halftime = _runtime / 2;
    float percenthalf = 0;

    for (size_t i = 0; i < _particles.size(); i++)
    {
        if (_totalelapsed > halftime) {
            percenthalf = elapsedms / halftime;
            _particles[i].fadeout(percenthalf);

            // change color
            switch (_type) {
                case ParticleSystemType::Explosion:
                    // explosion gets more red
                    _particles[i].addColor(glm::vec3(0.0, -percenthalf, 0.0));
                    break;
                case ParticleSystemType::PlayerExhaust:
                    // exhaust gets darker
                    _particles[i].addColor(glm::vec3(-percenthalf, 0.0, 0.0));
                    break;
                case ParticleSystemType::DeathFollowup:
                    // death gets darker quickly
                    _particles[i].addColor(glm::vec3(-percenthalf*2, 0.0, 0.0));
                default:
                    break;
            }
        }
        glm::vec3 velocity = _particles[i].getVelocity();
        glm::vec3 change = glm::vec3(velocity.x * elapsedms, velocity.y * elapsedms, velocity.z * elapsedms);
        _particles[i].setPosition(change + _particles[i].getPosition());
    }
    _totalelapsed += elapsedms;

    return (_totalelapsed < _runtime);
}

void ParticleSystem::hide() { _visible = false; }

void ParticleSystem::createExplosion(glm::vec3 position)
{
    _size = 250;
    _runtime = 1000.0;
    _particlesize = 5;

    for (int i = 0; i < _size; i++)
    {
        float randcolor = ((float)rand()/RAND_MAX) / 2.0;
        Particle p(glm::vec4(1.0, randcolor, 0.0, 1.0),                      // color
                    position,                                                // position
                    glm::vec3((0.0005 * ((float)rand()/RAND_MAX)) - 0.00025, // velocity
                              (0.0005 * ((float)rand()/RAND_MAX)) - 0.00025,
                              (0.0005 * ((float)rand()/RAND_MAX)) - 0.00025));
        _particles.push_back(p);
    }
}

void ParticleSystem::createPlayerExhaust(glm::vec3 position)
{
    _size = 2;
    _runtime = 500.0;
    _particlesize = 4;

    Particle p1(glm::vec4(0.8, 1.0, 1.0, 1.0),
                position + glm::vec3(-0.02, -0.05, 0.0),
                glm::vec3(0.0));
    Particle p2(glm::vec4(0.8, 1.0, 1.0, 1.0),
                position + glm::vec3(0.02, -0.05, 0.0),
                glm::vec3(0.0));
    _particles.push_back(p1);
    _particles.push_back(p2);
}

void ParticleSystem::createEnemyExhaust(glm::vec3 position)
{
    _size = 1;
    _runtime = 500.0;
    _particlesize = 3;

    Particle p(glm::vec4(0.8, 1.0, 0.8, 1.0),
               position + glm::vec3(0.0, 0.01, 0.0),
               glm::vec3(0.0, 0.05, 0.0));
    _particles.push_back(p);
}

void ParticleSystem::createDeath(glm::vec3 position)
{
    _size = 300;
    _runtime = 1000.0;
    _particlesize = 5;

    for (int i = 0; i < _size; i++)
    {
        float randcolor = ((float)rand()/RAND_MAX) / 2.0;
        Particle p(glm::vec4(0.0, 1.0, 1.0, 1.0),                      // color
                    position,                                                // position
                    glm::vec3((0.0008 * ((float)rand()/RAND_MAX)) - 0.0004, // velocity
                                (0.0008 * ((float)rand()/RAND_MAX)) - 0.0004,
                                (0.0008 * ((float)rand()/RAND_MAX)) - 0.0004));
        _particles.push_back(p);
    }
}

void ParticleSystem::createDeathFollowup(glm::vec3 position)
{
    _delay = 300;
    _size = 600;
    _runtime = 2000.0;
    _particlesize = 7;

    for (int i = 0; i < _size; i++)
    {
        float randcolor = ((float)rand()/RAND_MAX) / 2.0;
        Particle p(glm::vec4(0.9, 1.0, 1.0, 1.0),                      // color
                    position,                                                // position
                    glm::vec3((0.0008 * ((float)rand()/RAND_MAX)) - 0.0004, // velocity
                                (0.0008 * ((float)rand()/RAND_MAX)) - 0.0004,
                                (0.0008 * ((float)rand()/RAND_MAX)) - 0.0004));
        _particles.push_back(p);
    }
}