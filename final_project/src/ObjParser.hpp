#ifndef OBJPARSER_H
#define OBJPARSER_H

#include "Mesh.hpp"
#include <string>
#include <memory>

class ObjParser
{
public:
    static std::shared_ptr<Mesh> parse(const std::string &filename);
};

#endif
