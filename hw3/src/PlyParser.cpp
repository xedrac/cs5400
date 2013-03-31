#include <stdexcept>
#include <string.h>
#include <list>
#include "glm/glm.hpp"
#include "PlyParser.hpp"
#include "Util.hpp"

using namespace std;


shared_ptr<Mesh> parsePlyFile(const string &filename)
{
    struct Face {
        unsigned char nvertex;
        int *vertex;
    };

    // list of property information for a vertex
    PlyProperty vert_props[] = {  
        {(char *)"x", PLY_FLOAT, PLY_FLOAT, offsetof(glm::vec3,x), 0, 0, 0, 0},
        {(char *)"y", PLY_FLOAT, PLY_FLOAT, offsetof(glm::vec3,y), 0, 0, 0, 0},
        {(char *)"z", PLY_FLOAT, PLY_FLOAT, offsetof(glm::vec3,z), 0, 0, 0, 0},
    };

    // list of property information for a face
    PlyProperty face_props[] = {  
        {(char *)"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,vertex), 1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nvertex)},
    };

    int filetype;
    float version;
    int ntypes;
    char **typelist= NULL;

    PlyFile *ply = ply_open_for_reading((char *)filename.c_str(), &ntypes, &typelist, &filetype, &version);
    if (!ply) {
        throw std::runtime_error("Failed to open file");
    }

    auto mesh = make_shared<Mesh>(); 

    printf("Parsing PLY file '%s'\n", filename.c_str());
    //printf("Type: %d\n", filetype);
    //printf("Version: %f\n", version);

    if (filetype != PLY_ASCII) {
        ply_close(ply);
        throw std::runtime_error("Currently only ASCII PLY files are supported");
    }

    for (int i=0; i<ntypes; i++) {
        char *name = typelist[i];
        int nelem, nprop;
        ply_get_element_description(ply, name, &nelem, &nprop);

        if (!strncmp(name, "vertex", 6)) {
            ply_get_property(ply, name, &vert_props[0]);
            ply_get_property(ply, name, &vert_props[1]);
            ply_get_property(ply, name, &vert_props[2]); 

            glm::vec3 v;
            for (int j=0; j<nelem; j++) {
                ply_get_element(ply, (void *)&v);
                // Handle big-endian binary here
                //if (filetype == PLY_BINARY_BE) { 
                //    util::endianSwap(&v.x);
                //    util::endianSwap(&v.y);
                //    util::endianSwap(&v.z);
                //}

                mesh->vertices.push_back(glm::vec3(v.x, v.y, v.z));
            }
        }

        if (!strncmp(name, "face", 4)) {
            ply_get_property(ply, name, &face_props[0]);

            Face face;
            for (int j=0; j<nelem; j++) {
                ply_get_element(ply, (void *)&face);
                mesh->triangles.push_back(Triangle(face.vertex[0], face.vertex[1], face.vertex[2]));
            }
        }
    }

    ply_close(ply);
    printf("parsing complete\n");

    //mesh->calcNormalsMWE();
    //mesh->calcNormalsMWASEL();
    //mesh->calcNormalsMWA();
    mesh->calcNormalsMWSA();

    return mesh;
}


