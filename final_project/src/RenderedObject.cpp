#include <vector>
#include <GL/glew.h>
#include "RenderedObject.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;


RenderedObject::RenderedObject(GLuint program, std::shared_ptr<Mesh> mesh, GLfloat speed)
    : _mesh(mesh),
	  _position(glm::vec3(0.0, 0.0, 0.0)),
	  _rotation(glm::vec3(0.0, 0.0, 0.0)),
	  _speed(speed),
	  _direction(0),  // might want to be able to pass this in, but for now we'll default to one direction
      _visible(true),
      _texture("../textures/mycheckered_darkest.png")
{
    setModelMatrix(glm::mat4(1.0));

    glGenBuffers(1, &_glvertexbuffer);
    glGenBuffers(1, &_glnormalbuffer);
    glGenBuffers(1, &_glmeshbuffer);
    glGenBuffers(1, &_gluvbuffer);

    _glvertexattrib   = glGetAttribLocation (program, "vertex_coord");
    _glnormalattrib   = glGetAttribLocation (program, "vertex_normal");
    _gluvattrib       = glGetAttribLocation (program, "uv_coord");
    _glshininess      = glGetUniformLocation(program, "shininess");
    _glambient        = glGetUniformLocation(program, "materialambient");
    _gldiffuse        = glGetUniformLocation(program, "materialdiffuse");
    _glspecular       = glGetUniformLocation(program, "materialspecular");
    _gltexturesampler = glGetUniformLocation(program, "texturesampler");

    Material m;
    m.setAmbient(glm::vec3(0.2, 0.2, 0.2));
    m.setDiffuse(glm::vec3(0.8, 1.0, 0.8));
    m.setSpecular(glm::vec3(1.0, 1.0, 1.0));
    m.setShininess(100.0);
    setMaterial(m);

    storePoints();
    storeNormals();
    storeMesh();
    storeTextureCoords();
}

// Store the texture coordinates in a GPU buffer
// Currently we fake the coordinates with the 
// vertex data since PLY files don't provide them
void RenderedObject::storeTextureCoords()
{
    const vector<glm::vec3> &verts = _mesh->vertices;

    vector<GLfloat> uvcoords;
    for (size_t k=0; k<verts.size(); k++) {
        const glm::vec3 &v = verts[k];
        uvcoords.push_back(v.x*32);
        uvcoords.push_back(v.y*32);
    }   

    glBindBuffer(GL_ARRAY_BUFFER, _gluvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvcoords.size()*sizeof(GLfloat), &uvcoords[0], GL_STATIC_DRAW);
}


// Store the vertices in a GPU buffer
void RenderedObject::storePoints()
{
    const vector<glm::vec3> &verts = _mesh->vertices;

    vector<GLfloat> rawpoints;
    for (size_t k=0; k<verts.size(); k++) {
        const glm::vec3 &v = verts[k];
        rawpoints.push_back(v.x);
        rawpoints.push_back(v.y);
        rawpoints.push_back(v.z);
        rawpoints.push_back(1.0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _glvertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, rawpoints.size()*sizeof(GLfloat), rawpoints.data(), GL_STATIC_DRAW);
}


// Store the normals in a GPU buffer
void RenderedObject::storeNormals()
{
    const vector<glm::vec3> &normals = _mesh->normals;

    vector<GLfloat> rawnormals;
    for (size_t k=0; k<normals.size(); k++) {
        const glm::vec3 &n = normals[k];
        rawnormals.push_back(n.x);
        rawnormals.push_back(n.y);
        rawnormals.push_back(n.z);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _glnormalbuffer);
    glBufferData(GL_ARRAY_BUFFER, rawnormals.size()*sizeof(GLfloat), rawnormals.data(), GL_STATIC_DRAW);
}


// Store the mesh in a GPU buffer
void RenderedObject::storeMesh()
{
    const vector<Triangle> &triangles = _mesh->triangles;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glmeshbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size()*3*sizeof(GLuint), triangles.data(), GL_STATIC_DRAW);
}


// Rotate the object along the arbitrary axis, by 'theta' degrees
void RenderedObject::rotate(glm::vec3 axis, double theta)
{
    if (axis.x > 0) {
		_rotation.x += theta;
	}
	if (axis.y > 0) {
		_rotation.y += theta;
	}
	if (axis.z > 0) {
		_rotation.z += theta;
	}
}


// Move the object along the x, y, and z axes in the amount specified
void RenderedObject::translate(glm::vec3 xyz)
{
	_position += xyz;
}


// Objects that are not 'visible' will not be rendered
void RenderedObject::setVisible(bool visible)
{
    _visible = visible;
}


// Set the matrix to convert from model coords to world coords
void RenderedObject::setModelMatrix(glm::mat4 matrix)
{
    _modelmatrix = matrix; 
    _modelinvtranspmatrix = glm::inverseTranspose(glm::mat3(matrix));
}


// Set the material of the object
void RenderedObject::setMaterial(const Material &m)
{
    _material = m; 
}


// Render the object
void RenderedObject::render(GLuint modelmatrixid, GLuint modelinvtranspmatrixid)
{
    // Don't render if it's been flagged as not visible
    if (!_visible)
        return;

    _texture.bindTexture();

	// do model transformations
	glm::mat4 modelTranslate = glm::translate(_modelmatrix, _position);
	glm::mat4 modelRotateX = glm::rotate(modelTranslate, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 modelRotateY = glm::rotate(modelRotateX, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::rotate(modelRotateY, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(modelmatrixid, 1, GL_FALSE, glm::value_ptr(model));

    glUniformMatrix3fv(modelinvtranspmatrixid, 1, GL_FALSE, glm::value_ptr(_modelinvtranspmatrix));

    glEnableVertexAttribArray(_glvertexattrib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glmeshbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _glvertexbuffer);
    glVertexAttribPointer(_glvertexattrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(_glnormalattrib);
    glBindBuffer(GL_ARRAY_BUFFER, _glnormalbuffer);
    glVertexAttribPointer(_glnormalattrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(_gluvattrib);
    glBindBuffer(GL_ARRAY_BUFFER, _gluvbuffer);
    glVertexAttribPointer(_gluvattrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture.textureId());
    glUniform1i(_gltexturesampler, 0);

    glDrawElements(GL_TRIANGLES, _mesh->triangles.size()*3, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(_glvertexattrib);
    glDisableVertexAttribArray(_glnormalattrib);
    glDisableVertexAttribArray(_gluvattrib);

    GLfloat shininess  = _material.getShininess();
    glm::vec4 ambient  = glm::vec4(_material.getAmbient(), 1.0);
    glm::vec4 diffuse  = glm::vec4(_material.getDiffuse(), 1.0);
    glm::vec4 specular = glm::vec4(_material.getSpecular(), 1.0);

    glUniform1f(_glshininess, shininess);
    glUniform4f(_glambient, ambient.r, ambient.g, ambient.b, ambient.w);
    glUniform4f(_gldiffuse, diffuse.r, diffuse.g, diffuse.b, ambient.w);
    glUniform4f(_glspecular, specular.r, specular.g, specular.b, ambient.w);
}

// Update the object's state. Returns false if direction needs to change
bool RenderedObject::update(int msDiff)
{
	if (_direction == 0)
	{
		this->rotate(glm::vec3(0.0, 1.0, 0.0), this->_speed * 500 * msDiff);
		this->translate(glm::vec3(_speed * msDiff, 0.0, 0.0));
		return !(this->_position.x > 0.11);
	}
	else
	{
		this->rotate(glm::vec3(0.0, 1.0, 0.0), this->_speed * -500 * msDiff);
		this->translate(glm::vec3(-_speed * msDiff, 0.0, 0.0));
		return !(this->_position.x < -0.09);
	}

	// rules to change direction -- earlier discussion was if invisible bounding walls were hit
	// moved up into above logic branch to prevent sticky boundary problem
	//return !(this->_position.x > 0.11) && !(this->_position.x < -0.09);
}

// Change the object's direction
void RenderedObject::changeDirection()
{
	// simple 0 <-> 1 for now
	_direction = _direction ^ 1;
}
