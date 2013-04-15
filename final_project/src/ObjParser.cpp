#include "ObjParser.hpp"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;


shared_ptr<Mesh> ObjParser::parse(const string &filename)
{
    // OpenGL doesn't support multiple vertices referencing the
    // same normal index, but OBJ files do.  So this cache is to
    // keep track of which normals we have already referenced, so
    // that next time we reference it, we can just duplicate the 
    // normal.  That way the number of vertices and normals is the
    // same

    // Start the vectors with size 1 to use 1-based indexing until
    // the end.  This is for convenience because OBJ files using 1-based
    // indexing
    vector<GLuint> vertex_to_normal(1, 0);
    vector<GLuint> vertex_to_uvcoord(1, 0);

    vector<glm::vec3> normals(1);
    vector<glm::vec2> uvcoords(1);

    ifstream in(filename.c_str(), ios::in);
    if (!in) {
        throw std::runtime_error("Failed to open file");
    }

    auto mesh = make_shared<Mesh>();

    string line;
    size_t lineno = 0;

    while (getline(in, line)) {
        ++lineno;

        string key = line.substr(0,2);
        if (key == "v ") {
            istringstream s(line.substr(2));
            glm::vec3 vertex;
            s >> vertex.x;
            s >> vertex.y;
            s >> vertex.z;
            mesh->vertices.push_back(vertex);

            // Must be the same size as mesh->vertices
            vertex_to_normal.push_back(0);
            vertex_to_uvcoord.push_back(0);

        } else if (key == "vn") {
            istringstream s(line.substr(2));
            glm::vec3 normal;
            s >> normal.x;
            s >> normal.y;
            s >> normal.z;
            normals.push_back(normal);

        } else if (key == "vt") {
            istringstream s(line.substr(2));
            glm::vec3 uvcoord;
            s >> uvcoord.x;
            s >> uvcoord.y;
            s >> uvcoord.z;
            uvcoords.push_back(glm::vec2(uvcoord.x, uvcoord.y));

        } else if (key == "f ") {
            istringstream s(line.substr(2));
            GLuint v1=0, vt1=0, vn1=0;
            GLuint v2=0, vt2=0, vn2=0;
            GLuint v3=0, vt3=0, vn3=0;
    
            char c;
            s >> v1;  s.get(c);
            s >> vt1; s.get(c);
            s >> vn1; s.get(c);

            s >> v2;  s.get(c);
            s >> vt2; s.get(c);
            s >> vn2; s.get(c);

            s >> v3;  s.get(c);
            s >> vt3; s.get(c);
            s >> vn3;

            // duplicate normals if necessary so that
            // each vertex references a unique normal
            if (vertex_to_normal[v1] == 0) {
                vertex_to_normal[v1] = vn1;
            } else if (vertex_to_normal[v1] != vn1) {
                normals.push_back(normals[vn1]); 
                vn1 = normals.size()-1;
                vertex_to_normal[v1] = vn1; 
            }

            if (vertex_to_normal[v2] == 0) {
                vertex_to_normal[v2] = vn2;
            } else if (vertex_to_normal[v2] != vn2) {
                normals.push_back(normals[vn2]); 
                vn2 = normals.size()-1;
                vertex_to_normal[v2] = vn2; 
            }

            if (vertex_to_normal[v3] == 0) {
                vertex_to_normal[v3] = vn3;
            } else if (vertex_to_normal[v3] != vn3) {
                normals.push_back(normals[vn3]); 
                vn3 = normals.size()-1;
                vertex_to_normal[v3] = vn3; 
            }

            // Now the same for the UV coords
            if (vertex_to_uvcoord[v1] == 0) {
                vertex_to_uvcoord[v1] = vt1;
            } else if (vertex_to_uvcoord[v1] != vt1) {
                uvcoords.push_back(uvcoords[vt1]); 
                vt1 = uvcoords.size()-1;
                vertex_to_uvcoord[v1] = vt1; 
            }

            if (vertex_to_uvcoord[v2] == 0) {
                vertex_to_uvcoord[v2] = vt2;
            } else if (vertex_to_uvcoord[v2] != vt2) {
                uvcoords.push_back(uvcoords[vt2]); 
                vt2 = uvcoords.size()-1;
                vertex_to_uvcoord[v2] = vt2; 
            }

            if (vertex_to_uvcoord[v3] == 0) {
                vertex_to_uvcoord[v3] = vt3;
            } else if (vertex_to_uvcoord[v3] != vt3) {
                uvcoords.push_back(uvcoords[vt3]); 
                vt3 = uvcoords.size()-1;
                vertex_to_uvcoord[v3] = vt3; 
            }

            mesh->faces.push_back(Face(v1-1, v2-1, v3-1));

        } else if (line[0] == '#') {
            /* ignoring this line */

        } else {
            /* ignoring this line */
        }
    }

    // Now we simply need to populate the mesh's normals and uvcoords vectors
    // such that the indices align with mesh->vertices
    for (size_t i=0; i<mesh->vertices.size(); i++) {
        size_t vertexindex = i+1;  // Swatch back to 1-based indexing
        GLuint normalindex = vertex_to_normal[vertexindex];
        GLuint uvindex     = vertex_to_normal[vertexindex];

        mesh->normals .push_back(normals[normalindex]);
        mesh->uvcoords.push_back(uvcoords[uvindex]);
    }

    mesh->calcBoundingBox();
    return mesh;
}
