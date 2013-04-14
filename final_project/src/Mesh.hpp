#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include "glm/glm.hpp"
#include "BoundingBox.hpp"

struct Face;


class Mesh
{
public:
    void calcNormalsMWSA();
    void calcBoundingBox();

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvcoords;

    // vertex indicies that make up the faces of this mesh
    // (0,1,2), (3,4,5) ...  are indexes for faces
    std::vector<Face> faces;

    const BoundingBox &getBoundingBox() { return _boundingbox; }

private:
    std::vector<glm::vec3> calcFaceNormals(std::vector<std::vector<size_t> > &vertexToFacesMap);
    glm::vec3 calcNormalMWASEL(const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN);
    glm::vec3 calcNormalMWA(size_t vindex, const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN);
    glm::vec3 calcNormalMWE(const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN);
    glm::vec3 calcNormalMWSA(const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN);
    float calcAreaOfFace(const Face &t);

private:
    BoundingBox _boundingbox;
};






struct Face
{
    Face(GLuint aa, GLuint bb, GLuint cc)
        : a(aa), b(bb), c(cc)
    {
    }

    GLuint a;
    GLuint b;
    GLuint c;
};


#endif
