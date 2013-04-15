
#include <list>
#include <cmath>
#include <iostream>
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

#if 0
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
#endif


// Find the box surronding the mesh (in model space)
void Mesh::calcBoundingBox()
{
    Box3D box = Box3D();

    cout << "Model BoundingBox: ";    

    for (size_t i=0; i<vertices.size(); i++) {
        const glm::vec3 &v = vertices[i];           
        if      (v.x < box.x0) box.x0 = v.x;
        else if (v.x > box.x1) box.x1 = v.x;

        if      (v.y < box.y0) box.y0 = v.y;
        else if (v.y > box.y1) box.y1 = v.y;

        if      (v.z < box.z0) box.z0 = v.z;
        else if (v.z > box.z1) box.z1 = v.z;
    }

    cout << "(" << box.x0 << "," << box.x1 << ") "
         << "(" << box.y0 << "," << box.y1 << ") "
         << "(" << box.z0 << "," << box.z1 << ")\n";

    _boundingbox = BoundingBox(box);
}
