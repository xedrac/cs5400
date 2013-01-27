//#include <functional>
//#include <memory>

#include <iostream>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "Mountain.hpp"
#include "defs.h"

using namespace std;


Mountain::Mountain()
    : _rndnumgen(0.0, 1.0),
      _randomscale(0.1)
{
    _rndengine.seed(time(0));
}


void Mountain::setRandomScale(GLfloat scale)
{
    _randomscale = scale;
}


const vector<Triangle> &Mountain::getTriangles()
{
    return _triangles;
}


const vector<glm::vec3> &Mountain::getVertexNormals()
{
    return _vertexnormals;
}

// Generate a random unit vector
Point Mountain::randVector()
{
    return Point(randFloat(), randFloat(), randFloat());
}


// Generates a random float between 0 and 1
GLfloat Mountain::randFloat()
{
    return _rndnumgen(_rndengine);
}


// Returns a randomized mid-point between a and b.  The mid-point
// is restricted to _randomscale * length(b-a) from the center.
// The result is cached in order to prevent tearing
Point Mountain::randomMidPoint(const Point &a, const Point &b)
{
    // sort the points first so we don't have to store both permutations
    const Point &aa = (a<b? a: b);
    const Point &bb = (a<b? b: a);
    Edge edge(aa, bb);

    auto itr = _midpoints.find(edge);
    if (itr != _midpoints.end())
        return itr->second;

    GLfloat scalefactor = _randomscale * a.distanceTo(b);
    Point mid = (a+b)/2.0 + randVector() * scalefactor;
    _midpoints.insert(make_pair(edge, mid));
    return mid;
}


// Calculate the vertex normal as the sum of the triangle
// normals that surround this vertex
glm::vec3 Mountain::calcVertexNormal(const Point &p)
{
    auto itr = _pointmap.find(p);
    if (itr == _pointmap.end())
        return glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 n(0.0f, 0.0f, 0.0f);

    const vector<size_t> &v = itr->second;
    for (size_t i=0; i<v.size(); i++) {
        n += _trianglenormals[v[i]];   
    }

    return n;
}


// recursively generate a randomized mountain side
// from a simple triangle.  Add the generated triangles
// to the triangles vector, along with their normals
void Mountain::generateMountainSide(const Point &a,
                                    const Point &b,
                                    const Point &c,
                                    int k)
{
    if (k>0) {
        Point ab = randomMidPoint(a, b);
        Point ac = randomMidPoint(a, c);
        Point bc = randomMidPoint(b, c);

        generateMountainSide( a, ab, ac, k-1);
        generateMountainSide(ab,  b, bc, k-1);
        generateMountainSide(ac, bc,  c, k-1);
        generateMountainSide(ab, bc, ac, k-1);
    } else {
        Triangle t(a, b, c);
        _triangles.push_back(t);
        _trianglenormals.push_back(t.calcNormal());

        size_t index = _triangles.size()-1;
        _pointmap[a].push_back(index);
        _pointmap[b].push_back(index);
        _pointmap[c].push_back(index);
    }
}


void Mountain::generate(const Point &a,
                        const Point &b,
                        const Point &c,
                        const Point &d,
                        int k)
{
    _triangles.clear();
    _trianglenormals.clear();
    _vertexnormals.clear();

    cout << endl << "Generating fractal mountain..." << endl;

    generateMountainSide(a, b, c, k);
    //generateMountainSide(a, c, d, k);
    //generateMountainSide(a, d, b, k);
    //generateMountainSide(b, d, c, k);
   
    cout << "Computing vertex normals..." << endl;

    // Now calculate the vertex normals so we can do some
    // half-decent shading in the fragment shader.  Usually
    // these would be read from file and wouldn't need to be
    // calculated
    std::map<Point, glm::vec3> normalcache;

    for (size_t i=0; i<_triangles.size(); i++) {
        const Triangle &t = _triangles[i];         

        if (normalcache.count(t.a) > 0) {
            _vertexnormals.push_back(normalcache[t.a]);
        } else {
            glm::vec3 n = glm::normalize(calcVertexNormal(t.a));
            _vertexnormals.push_back(n);
            normalcache.insert(make_pair(t.a, n));
        }

        if (normalcache.count(t.b) > 0) {
            _vertexnormals.push_back(normalcache[t.b]);
        } else {
            glm::vec3 n = glm::normalize(calcVertexNormal(t.b));
            _vertexnormals.push_back(n);
            normalcache.insert(make_pair(t.b, n));
        }

        if (normalcache.count(t.c) > 0) {
            _vertexnormals.push_back(normalcache[t.c]);
        } else {
            glm::vec3 n = glm::normalize(calcVertexNormal(t.c));
            _vertexnormals.push_back(n);
            normalcache.insert(make_pair(t.c, n));
        }
    }
} 


