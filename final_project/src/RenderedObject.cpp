#include <vector>
#include <GL/glew.h>
#include "RenderedObject.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;


RenderedObject::RenderedObject(GLuint program,
                               std::shared_ptr<Mesh> mesh,
                               glm::vec3 position,
                               glm::vec3 scale,
                               glm::vec3 rotation)
    : _id(INVALID_OBJECTID),
      _mesh(mesh),
      _boundingbox(mesh->getBoundingBox()),
	  _position(position),
	  _initialrotation(rotation),
      _rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
      _scale(scale),
      _visible(true),
      _applytexture(true),
      _applylighting(true),
      //_showbounds(false),
      _texture(nullptr)
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
    _gltextureswitch  = glGetUniformLocation(program, "textureswitch");
    _gllightswitch    = glGetUniformLocation(program, "lightswitch");

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

    calculateModelMatrix();
}


void RenderedObject::setMaterial(const Material &m)
{
    _material = m;
}

void RenderedObject::setTexture(const string &texturefile)
{
    _texture = make_shared<Texture>(texturefile);
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
        uvcoords.push_back((v.x + 1.0) / 2.0);
        uvcoords.push_back((v.y + 1.0) / 2.0);
    }   

    glBindBuffer(GL_ARRAY_BUFFER, _gluvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvcoords.size()*sizeof(GLfloat), &uvcoords[0], GL_STATIC_DRAW);
}


#if 0
void RenderedObject::addBoundPoint(const glm::vec3 &p)
{
    _boundpoints.push_back(p.x);
    _boundpoints.push_back(p.y);
    _boundpoints.push_back(p.z);
    _boundpoints.push_back(1.0);
}

void RenderedObject::storeBoundsPoints()
{
    Box3D bounds = _boundingbox.getBounds();
    glm::vec3 p1(bounds.x0, bounds.y0, bounds.z0);
    glm::vec3 p2(bounds.x1, bounds.y0, bounds.z0);
    glm::vec3 p3(bounds.x0, bounds.y1, bounds.z0);
    glm::vec3 p4(bounds.x1, bounds.y1, bounds.z0);
    glm::vec3 p5(bounds.x0, bounds.y0, bounds.z1);
    glm::vec3 p6(bounds.x1, bounds.y0, bounds.z1);
    glm::vec3 p7(bounds.x0, bounds.y1, bounds.z1);
    glm::vec3 p8(bounds.x1, bounds.y1, bounds.z1);

    _boundpoints.clear();
    addBoundPoint(p1);
    addBoundPoint(p2);
    addBoundPoint(p4);
    addBoundPoint(p3);
    addBoundPoint(p1);
    addBoundPoint(p5);
    addBoundPoint(p7);
    addBoundPoint(p8);
    addBoundPoint(p6);
    addBoundPoint(p5);
    addBoundPoint(p7);
    addBoundPoint(p3);
    addBoundPoint(p4);
    addBoundPoint(p8);
    addBoundPoint(p6);
    addBoundPoint(p2);

    glBindBuffer(GL_ARRAY_BUFFER, _glvertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, _boundpoints.size()*sizeof(GLfloat), _boundpoints.data(), GL_STATIC_DRAW);
}
#endif


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
    const vector<Face> &faces = _mesh->faces;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glmeshbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size()*3*sizeof(GLuint), faces.data(), GL_STATIC_DRAW);
}

// Rotate the object along the arbitrary axis, by 'theta' degrees
void RenderedObject::rotate(glm::vec3 axis, float theta)
{
    if (axis.x > 0) { _rotation.x += theta; }
	if (axis.y > 0) { _rotation.y += theta; }
	if (axis.z > 0) { _rotation.z += theta; }
    calculateModelMatrix();
}


// Move the object along the x, y, and z axes in the amount specified
void RenderedObject::translate(glm::vec3 xyz)
{
	_position += xyz;
    calculateModelMatrix();
}


void RenderedObject::scale(glm::vec3 scalevec)
{
    _scale = scalevec; 
    calculateModelMatrix();
}


bool RenderedObject::intersects(const object_t &other)
{
    return _boundingbox.intersects(other->getBoundingBox());
}


// Render the object
void RenderedObject::render(GLuint modelmatrixid, GLuint modelinvtranspmatrixid)
{
    // Don't render if it's been flagged as not visible
    if (!_visible)
        return;

    if (_texture && _applytexture)
        _texture->bindTexture();

    glUniformMatrix4fv(modelmatrixid, 1, GL_FALSE, glm::value_ptr(_modelmatrix));
    glUniformMatrix3fv(modelinvtranspmatrixid, 1, GL_FALSE, glm::value_ptr(_modelinvtranspmatrix));

    glUniform1i(_gltextureswitch, (_applytexture? 1: 0));
    glUniform1i(_gllightswitch,   (_applylighting? 1: 0));

    if (_applylighting) {
        GLfloat shininess  = _material.getShininess();
        glm::vec4 ambient  = glm::vec4(_material.getAmbient(), 1.0);
        glm::vec4 diffuse  = glm::vec4(_material.getDiffuse(), 1.0);
        glm::vec4 specular = glm::vec4(_material.getSpecular(), 1.0);

        glUniform1f(_glshininess, shininess);
        glUniform4f(_glambient,  ambient.r,  ambient.g,  ambient.b,  ambient.w);
        glUniform4f(_gldiffuse,  diffuse.r,  diffuse.g,  diffuse.b,  ambient.w);
        glUniform4f(_glspecular, specular.r, specular.g, specular.b, ambient.w);
    }

    if (_texture && _applytexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _texture->textureId());
        glUniform1i(_gltexturesampler, 0);
    }

    //if (_showbounds)
    //    storePoints();

    glEnableVertexAttribArray(_glvertexattrib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _glmeshbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _glvertexbuffer);
    glVertexAttribPointer(_glvertexattrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(_glnormalattrib);
    glBindBuffer(GL_ARRAY_BUFFER, _glnormalbuffer);
    glVertexAttribPointer(_glnormalattrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (_applytexture) {
        glEnableVertexAttribArray(_gluvattrib);
        glBindBuffer(GL_ARRAY_BUFFER, _gluvbuffer);
        glVertexAttribPointer(_gluvattrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    glDrawElements(GL_TRIANGLES, _mesh->faces.size()*3, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(_glvertexattrib);
    glDisableVertexAttribArray(_glnormalattrib);
    glDisableVertexAttribArray(_gluvattrib);

#if 0
    // Draw the bounding box
    if (_showbounds) {
        storeBoundsPoints();
        glEnableVertexAttribArray(_glvertexattrib);
        glBindBuffer(GL_ARRAY_BUFFER, _glvertexbuffer);
        glVertexAttribPointer(_glvertexattrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINE_LOOP, 0, _boundpoints.size()/sizeof(GLfloat));
        glDisableVertexAttribArray(_glvertexattrib);
    }
#endif
}


void RenderedObject::calculateModelMatrix()
{
    glm::mat4 matrix = glm::mat4(1.0);
    GLfloat rotx = _initialrotation.x + _rotation.x;
    GLfloat roty = _initialrotation.y + _rotation.y;
    GLfloat rotz = _initialrotation.z + _rotation.z;

    matrix = glm::translate(matrix, _position);
    matrix = glm::rotate(matrix, rotz, glm::vec3(0, 0, 1));
    matrix = glm::rotate(matrix, roty, glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, rotx, glm::vec3(1, 0, 0));
    matrix = glm::scale (matrix, _scale);

    _modelmatrix = matrix;
    _modelinvtranspmatrix = glm::inverseTranspose(glm::mat3(matrix));
}

