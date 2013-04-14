
#include <list>
#include <cmath>
#include "Mesh.hpp"

using namespace std;


// Calculate the face normals for this mesh
vector<glm::vec3> Mesh::calcFaceNormals(vector<vector<size_t> > &vertexToFacesMap)
{
    // keep track of which faces a given vertex is part of
    vertexToFacesMap.resize(vertices.size());

    // calculate face normals
    vector<glm::vec3> tNormals;
    for (size_t i=0; i<faces.size(); i++) {
        const Face &t = faces[i];
        const glm::vec3 &a = vertices[t.a];
        const glm::vec3 &b = vertices[t.b];
        const glm::vec3 &c = vertices[t.c];
        tNormals.push_back(glm::cross(b-a, c-a));

        vertexToFacesMap[t.a].push_back(i);
        vertexToFacesMap[t.b].push_back(i);
        vertexToFacesMap[t.c].push_back(i);
    }

    return tNormals;
}

float Mesh::calcAreaOfFace(const Face &t)
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
        const Face &t = faces[tindex];
        float area = calcAreaOfFace(t);
        normal += tN[tindex]*area;
    }

    return glm::normalize(normal);
}

void Mesh::calcNormalsMWSA()
{
    vector<vector<size_t>> vtmap;
    vector<glm::vec3> tN = calcFaceNormals(vtmap);
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
