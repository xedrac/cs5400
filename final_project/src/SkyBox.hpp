#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <GL/glew.h>
#include "FlatSurface.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include <string>
#include <memory>


class SkyBox
{
public:
    // paths to image files
    SkyBox(GLuint program,
           const std::string &topimage,
           const std::string &botimage,
           const std::string &leftimage,
           const std::string &rightimage,
           const std::string &frontimage,
           const std::string &backimage);

    void addToScene(Scene &scene);

private:
    void loadMesh(const std::string &filename);

private:
    std::shared_ptr<Mesh> _mesh;

    std::shared_ptr<FlatSurface> _top;
    std::shared_ptr<FlatSurface> _bot;
    std::shared_ptr<FlatSurface> _left;
    std::shared_ptr<FlatSurface> _right;
    std::shared_ptr<FlatSurface> _front;
    std::shared_ptr<FlatSurface> _back;
};


#endif

