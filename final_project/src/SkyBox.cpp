
#include "SkyBox.hpp"
#include "PlyParser.hpp"
#include "ObjParser.hpp"
#include <string>
#include <iostream>

using namespace std;


SkyBox::SkyBox(GLuint program,
               const std::string &topimage,
               const std::string &botimage,
               const std::string &leftimage,
               const std::string &rightimage,
               const std::string &frontimage,
               const std::string &backimage)
{
    loadMesh("models/flat.ply");

    _top   = make_shared<FlatSurface>(program, _mesh, glm::vec3( 0.0f, 5.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(90.0f, 0.0f, 0.0f));
    _bot   = make_shared<FlatSurface>(program, _mesh, glm::vec3( 0.0f,-5.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(-90.0f, 0.0f, 0.0f));
    _left  = make_shared<FlatSurface>(program, _mesh, glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 90.0f, 0.0f));
    _right = make_shared<FlatSurface>(program, _mesh, glm::vec3( 5.0f, 0.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f,-90.0f, 0.0f));
    _front = make_shared<FlatSurface>(program, _mesh, glm::vec3( 0.0f, 0.0f,-5.0f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    _back  = make_shared<FlatSurface>(program, _mesh, glm::vec3( 0.0f, 0.0f, 5.0f), glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    _top  ->setTexture(topimage);
    _bot  ->setTexture(botimage);
    _left ->setTexture(leftimage);
    _right->setTexture(rightimage);
    _front->setTexture(frontimage);
    _back ->setTexture(backimage);

    _top  ->setLightSwitch(false);
    _bot  ->setLightSwitch(false);
    _left ->setLightSwitch(false);
    _right->setLightSwitch(false);
    _front->setLightSwitch(false);
    _back ->setLightSwitch(false);
}


void SkyBox::addToScene(Scene &scene)
{
    _top  ->setId(scene.getNewObjectId()); 
    _bot  ->setId(scene.getNewObjectId()); 
    _left ->setId(scene.getNewObjectId()); 
    _right->setId(scene.getNewObjectId()); 
    _front->setId(scene.getNewObjectId()); 
    _back ->setId(scene.getNewObjectId()); 

    scene.insertObject(_top);    
    scene.insertObject(_bot);    
    scene.insertObject(_left);    
    scene.insertObject(_right);    
    scene.insertObject(_front);    
    scene.insertObject(_back);    
}


void SkyBox::loadMesh(const std::string &filename)
{
    if (filename.find(".ply") != string::npos) {
        _mesh = parsePlyFile(filename);
    } else if (filename.find(".obj") != string::npos) {
        _mesh = ObjParser::parse(filename);
    } else {
        cerr << "Unable to load unknown file type for SkyBox" << endl;
    }
}

