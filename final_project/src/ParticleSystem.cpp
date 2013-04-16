#include "ParticleSystem.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

ParticleSystem::ParticleSystem(GLuint program, ParticleSystemType type, GLfloat particlesize, glm::vec3 startposition)
{
    if (type == ParticleSystemType::Explosion)
    {
        _size = 250;
        _runtime = 1000.0;

        for (int i = 0; i < _size; i++)
        {
            float randcolor = ((float)rand()/RAND_MAX) / 3.0;
            Particle p(glm::vec4(1.0, randcolor, 0.0, 1.0),                     // color
                       startposition,                                           // position
                       glm::vec3((0.0005 * ((float)rand()/RAND_MAX)) - 0.00025, // velocity
                                 (0.0005 * ((float)rand()/RAND_MAX)) - 0.00025,
                                 (0.0005 * ((float)rand()/RAND_MAX)) - 0.00025));
            _particles.push_back(p);
        }
    }

    _particleprogram = program;
    _particlesize = particlesize;

    glGenBuffers(1, &_glvertexbuffer);
    glGenBuffers(1, &_glcolorbuffer);

    _glvertexattrib   = glGetAttribLocation (program, "vPosition");
    _glcolorattrib    = glGetAttribLocation (program, "vColor");

    _totalelapsed = 0;
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

    for (size_t i = 0; i < _size; i++)
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
}

// Returns false if system has exceeded runtime
bool ParticleSystem::update(int elapsedms)
{
    // Don't update if it's been flagged as not visible
    if (!_visible)
        return true;

    float halftime = _runtime / 2;
    float percenthalf = 0;

    for (size_t i = 0; i < _particles.size(); i++)
    {
        if (_totalelapsed > halftime) {
            percenthalf = elapsedms / halftime;
            _particles[i].fadeout(percenthalf);
        }
        glm::vec3 velocity = _particles[i].getVelocity();
        glm::vec3 change = glm::vec3(velocity.x * elapsedms, velocity.y * elapsedms, velocity.z * elapsedms);
        _particles[i].setPosition(change + _particles[i].getPosition());
    }
    _totalelapsed += elapsedms;
    return (_totalelapsed < _runtime);
}

void ParticleSystem::hide() { _visible = false; }