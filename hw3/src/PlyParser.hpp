#ifndef PARSEPLYWRAPPER_H
#define PARSEPLYWRAPPER_H

#include <memory>
#include <string>
#include "Mesh.hpp"
#include "parsers/ply/ply.h"

std::shared_ptr<Mesh> parsePlyFile(const std::string &filename);

#endif
