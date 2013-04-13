
#include <list>
#include <cmath>
#include "Mesh.hpp"

using namespace std;


// Calculate the triangle normals for this mesh
vector<glm::vec3> Mesh::calcTriangleNormals(vector<vector<size_t> > &vertexToTrianglesMap)
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

// Naive method to calculate xy square around object
void Mesh::calcBoundingBox()
{
	// x_min, x_max, y_min, y_max
	glm::vec4 result = glm::vec4(1000, -1000, 1000, -1000);
	glm::vec3* item;
	for (size_t i=0; i<vertices.size(); i++) {
		item = &vertices[i];
		if (item->x < result.x)
			result.x = item->x;
		else if (item-> x > result.y)
			result.y = item->x;
		if (item->y < result.z)
			result.z = item->y;
		else if (item->y > result.w)
			result.w = item->y;
    }

    _boundingbox = BoundingBox(result);
}
