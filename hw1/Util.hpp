#ifndef UTIL_H_
#define UTIL_H_

#include <GL/glut.h>

namespace util
{
    // Floating point comparison functions
    bool floatEq(GLfloat a, GLfloat b);
    bool floatLt(GLfloat a, GLfloat b);
    bool floatGt(GLfloat a, GLfloat b);

} // util namespace

#endif
