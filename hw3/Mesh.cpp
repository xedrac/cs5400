
#include <list>
#include <cmath>
#include "Mesh.hpp"

using namespace std;


// Calculate the triangle normals for this mesh
// Also keep track of which triangles a given vertex
// is a part of 
vector<glm::vec3> Mesh::calcTriangleNormals(vector<vector<size_t>> &vertexToTrianglesMap)
{
    // keep track of which triangles a given vertex is part of
    vertexToTrianglesMap.resize(vertices.size());

    // calculate triangle normals
    vector<glm::vec3> tNormals;
    for (size_t i=0; i<triangles.size(); i++) {
        const Triangle &t = triangles[i];
        const glm::vec3 &a = vertices[t.a];
        const glm::vec3 &b = vertices[t.b];
        const glm::vec3 &c = vertices[t.c];
        tNormals.push_back(glm::cross(b-a, c-a));

        vertexToTrianglesMap[t.a].push_back(i);
        vertexToTrianglesMap[t.b].push_back(i);
        vertexToTrianglesMap[t.c].push_back(i);
    }

    return tNormals;
}


glm::vec3 Mesh::calcNormalMWA(size_t vindex, const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN)
{
    glm::vec3 normal(0.0, 0.0, 0.0);

    for (size_t i=0; i<tvec.size(); i++) {
        const size_t tindex = tvec[i];
        const Triangle &t = triangles[tindex];

        glm::vec3 p1, p2, p3;

        if (t.a == vindex) {
            p1 = vertices[t.a];    
            p2 = vertices[t.b];    
            p3 = vertices[t.c];    
        } else if (t.b == vindex) {
            p1 = vertices[t.b];    
            p2 = vertices[t.a];    
            p3 = vertices[t.c];    
        } else {
            p1 = vertices[t.c];
            p2 = vertices[t.a];    
            p3 = vertices[t.b];    
        }

        glm::vec3 edge1 = p2 - p1;
        glm::vec3 edge2 = p3 - p1;
        glm::vec3 edgecross = glm::cross(edge1, edge2);
        float sinalpha = glm::length(edgecross) / (glm::length(edge1) * glm::length(edge2));
        float alpha    = asin(sinalpha);

        normal += alpha * tN[tindex];
    }

    return glm::normalize(normal);
}


glm::vec3 Mesh::calcNormalMWASEL(const vector<size_t> &tvec, const vector<glm::vec3> &tN)
{
    glm::vec3 normal(0.0, 0.0, 0.0);

    for (size_t i=0; i<tvec.size(); i++) {
        const size_t tindex = tvec[i];
        const Triangle &t = triangles[tindex];
        const glm::vec3 &p1 = vertices[t.a];
        const glm::vec3 &p2 = vertices[t.b];
        const glm::vec3 &p3 = vertices[t.c];
        glm::vec3 edge1 = p2 - p1;
        glm::vec3 edge2 = p3 - p1;
        glm::vec3 edgecross = glm::cross(edge1, edge2);
        float edge1len = glm::length(edge1);
        float edge2len = glm::length(edge2);
        float sinalpha = glm::length(edgecross) / (edge1len * edge2len);
        normal += sinalpha * tN[tindex] / (edge1len * edge2len);
    }

    return glm::normalize(normal);
}


glm::vec3 Mesh::calcNormalMWE(const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN)
{
    glm::vec3 normal(0.0, 0.0, 0.0);

    for (size_t i=0; i<tvec.size(); i++) {
        const size_t tindex = tvec[i];
        normal += tN[tindex];
    }

    return glm::normalize(normal);
}


float Mesh::calcAreaOfTriangle(const Triangle &t)
{
    const glm::vec3 &p1 = vertices[t.a];
    const glm::vec3 &p2 = vertices[t.b];
    const glm::vec3 &p3 = vertices[t.c];
    double a = glm::length(p3-p2);
    double b = glm::length(p3-p1);
    double c = glm::length(p2-p1);

    double s = (a + b + c) / 2.0;

    return (float)sqrt(s*(s-a)*(s-b)*(s-c));
}

// Mean weighted by surface area
glm::vec3 Mesh::calcNormalMWSA(const std::vector<size_t> &tvec, const std::vector<glm::vec3> &tN)
{
    glm::vec3 normal(0.0, 0.0, 0.0); 

    for (size_t i=0; i<tvec.size(); i++) {
        const size_t tindex = tvec[i];
        const Triangle &t = triangles[tindex];
        float area = calcAreaOfTriangle(t);
        normal += tN[tindex]*area;
    }

    return glm::normalize(normal);
}


// Calculate the normals for a given mesh using the
// "Mean Weighted by Angle" (MWA) algorithm as seen in the paper
// "A Comparison of Algorithms for Vertex Normal Computation"
void Mesh::calcNormalsMWA()
{
    vector<vector<size_t>> vtmap;
    vector<glm::vec3> tN = calcTriangleNormals(vtmap);

    normals.clear();

    for (size_t i=0; i<vertices.size(); i++) {
        const vector<size_t> &tvec = vtmap[i]; 
        normals.push_back(calcNormalMWA(i, tvec, tN));
    }
}


// Calculate the normals for a given mesh using the
// "Mean Weighted by Angle" (MWASEL) algorithm as seen in the paper
// "A Comparison of Algorithms for Vertex Normal Computation"
void Mesh::calcNormalsMWASEL()
{
    vector<vector<size_t>> vtmap;
    vector<glm::vec3> tN = calcTriangleNormals(vtmap);

    normals.clear();

    for (size_t i=0; i<vertices.size(); i++) {
        const vector<size_t> &tvec = vtmap[i]; 
        normals.push_back(calcNormalMWASEL(tvec, tN));
    }
}


// Calculate the normals for a given mesh using the
// "Mean Weighted Equally" (MWE) algorithm (Naive approach)
void Mesh::calcNormalsMWE()
{
    vector<vector<size_t>> vtmap;
    vector<glm::vec3> tN = calcTriangleNormals(vtmap);

    normals.clear();

    for (size_t i=0; i<vertices.size(); i++) {
        const vector<size_t> &tvec = vtmap[i]; 
        normals.push_back(calcNormalMWE(tvec, tN));
    }
}

void Mesh::calcNormalsMWSA()
{
    vector<vector<size_t>> vtmap;
    vector<glm::vec3> tN = calcTriangleNormals(vtmap);

    normals.clear();

    for (size_t i=0; i<vertices.size(); i++) {
        const vector<size_t> &tvec = vtmap[i]; 
        normals.push_back(calcNormalMWSA(tvec, tN));
    }
}
