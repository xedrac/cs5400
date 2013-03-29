#ifndef DEFS_H_
#define DEFS_H_

#include <functional>
#include <glm/glm.hpp>
#include "Util.hpp"

class Point
{
public:
    Point(const GLfloat &a, const GLfloat &b, const GLfloat &c)
        : x(a), y(b), z(c)
    {
    }

    // While this isn't the best comparison function,
    // it considers all dimensions and works correctly
    // even if not optimally
    bool operator<(const Point &b) const
    {
        if      (util::floatLt(x, b.x)) return true;
        else if (util::floatGt(x, b.x)) return false;
        else if (util::floatLt(y, b.y)) return true;
        else if (util::floatGt(y, b.y)) return false;
        else if (util::floatLt(z, b.z)) return true;
        return false;
    }

    bool  operator==(const Point &other) const
    {
        return (util::floatEq(x, other.x) &&
                util::floatEq(y, other.y) &&
                util::floatEq(z, other.z));
    }

    Point operator+ (const Point &other)   const { return Point(x+other.x, y+other.y, z+other.z); }
    Point operator- (const Point &other)   const { return Point(x-other.x, y-other.y, z-other.z); }
    Point operator* (const GLfloat &value) const { return Point(x*value, y*value, z*value); }
    Point operator/ (const GLfloat &value) const { return Point(x/value, y/value, z/value); }

    Point &operator+=(const Point &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Point &operator/=(const GLfloat value) {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    GLfloat distanceTo(const Point &p) const {
        GLfloat dx = p.x - x;
        GLfloat dy = p.y - y;
        GLfloat dz = p.z - z;
        return sqrt(dx*dx + dy*dy + dz*dz);
    }

    glm::vec3 vec3() const {
        return glm::vec3(x, y, z);
    }

public:
    GLfloat x;
    GLfloat y;
    GLfloat z;
};



class Edge
{
public:
    Edge(const Point &p1, const Point &p2)
        : a(p1), b(p2)
    {
    }

    bool operator<(const Edge &other) const
    {
        return (getA() < other.a || getB() < other.b);
    }

    bool operator==(const Edge &other) const
    {
        return (getA() == other.a && getB() == other.b) ||
               (getA() == other.b && getB() == other.a);
    }

    Point getA() const { return a; }
    Point getB() const { return b; }

public:
    Point a;
    Point b;
};


class Triangle3
{
public:
    Triangle3(const Point &p1, const Point &p2, const Point &p3)
        : a(p1), b(p2), c(p3)
    {
    }

    glm::vec3 calcNormal() const
    {
        //return glm::cross((b-a).vec3(), (c-a).vec3());
        glm::vec3 a3 = a.vec3();
        glm::vec3 b3 = b.vec3();
        glm::vec3 c3 = c.vec3();
        return glm::normalize(glm::cross(b3-a3, c3-a3));
    }

    Point a;
    Point b;
    Point c;
};


// This was taken from Boost and should have been included
// in the C++11 standard.  It can hash multiple elements
// into a single hash
template <class T>
inline void hashCombine(size_t &seed, const T &v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct EdgeHash
{
    size_t operator()(const Edge &edge) const {
        size_t seed = 0;
        hashCombine(seed, edge.a.x);
        hashCombine(seed, edge.a.y);
        hashCombine(seed, edge.a.z);
        hashCombine(seed, edge.b.x);
        hashCombine(seed, edge.b.y);
        hashCombine(seed, edge.b.z);
        return seed;
    }
};


struct EdgeEqualTo //: std::binary_function<Edge, Edge, bool>
{
    bool operator()(const Edge &e1, const Edge &e2) const
    {
        return (e1.a == e2.a && e1.b == e2.b);
    }
};

#endif
