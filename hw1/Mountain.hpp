#ifndef MOUNTAIN_H_
#define MOUNTAIN_H_

#include <vector>
#include <random>
#include <map>
#include <unordered_map>
#include "glm/glm.hpp"
#include "defs.h"


// Used to generate a simple fractal 3D mountain

class Mountain
{
public:
    Mountain();

    void setRandomScale(GLfloat scale);

    void generate(const Point &a,
                  const Point &b,
                  const Point &c,
                  const Point &d,
                  int k);

    // returns the vector of generated triangles (must call generate first)
    const std::vector<Triangle> &getTriangles();

    // 3 vertex normals per triangle - maps directly to _triangles
    const std::vector<glm::vec3> &getVertexNormals();

private:
    Point randVector();
    GLfloat randFloat();
    Point randomMidPoint(const Point &a, const Point &b);

    // Calculate the vertex normal as the sum of the triangle
    // normals that surround this vertex
    glm::vec3 calcVertexNormal(const Point &p);

    // recursively generate a randomized mountain side
    // from a simple triangle.  Add the generated triangles
    // to the triangles vector, along with their normals
    void generateMountainSide(const Point &a,
                              const Point &b,
                              const Point &c,
                              int k);

private:
    // generate populates this with the random mountain triangles
    std::vector<Triangle> _triangles;
    std::vector<glm::vec3> _trianglenormals;

    // Used to remember the random midpoint calculation for a given edge
    std::unordered_map<Edge, Point, EdgeHash, EdgeEqualTo> _midpoints;
    
    // shows which triangles a given vertex is part of
    std::map<Point, std::vector<size_t>> _pointmap;

    // the normals vectors for each vertex (3 per triangle)
    std::vector<glm::vec3> _vertexnormals;

    // random number generator
    std::mt19937 _rndengine;
    std::uniform_real_distribution<GLfloat> _rndnumgen;

    GLfloat _randomscale;
};


#endif
