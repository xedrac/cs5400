#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include "glm/glm.hpp"

struct Triangle {
    Triangle(GLuint aa, GLuint bb, GLuint cc)
        : a(aa), b(bb), c(cc)
    {
    }

    GLuint a;
    GLuint b;
    GLuint c;
};



class Mesh {
public:
    // Calculate the normals of the mesh using the
    // Mean Weighted by Angle algorithm
    // This will clear and populate the normals vector
    void calcNormalsMWA();

    // Calculate the normals of the mesh using the
    // Mean Weighted Average By Sine And Edge Reciprocal
    // This will clear and populate the normals vector
    void calcNormalsMWASEL();

    // Naive wait to calculate normals
    void calcNormalsMWE();

    void calcNormalsMWSA();

    // Complete set of vertices in the mesh
    std::vector<glm::vec3> vertices;

    // vertex normals
    std::vector<glm::vec3> normals;

    // vertex indicies that make up the triangles of this mesh
    // (0,1,2), (3,4,5) ...  are indexes for triangles
    std::vector<Triangle> triangles;

	glm::vec4 calcBounds();

 private:
    std::vector<glm::vec3> calcTriangleNormals(std::vector<std::vector<size_t>> &vertexToTrianglesMap);
    glm::vec3 calcNormalMWASEL(const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN);
    glm::vec3 calcNormalMWA(size_t vindex, const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN);
    glm::vec3 calcNormalMWE(const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN);
    glm::vec3 calcNormalMWSA(const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN);
    float     calcAreaOfTriangle(const Triangle &t);
};

#endif
