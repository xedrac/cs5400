#ifndef UTIL_H_
#define UTIL_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include <algorithm>

namespace util
{
    // Floating point comparison functions
    bool floatEq(GLfloat a, GLfloat b);
    bool floatLt(GLfloat a, GLfloat b);
    bool floatGt(GLfloat a, GLfloat b);

    template<class T>
    void endianSwap(T *obj)
    {
        unsigned char *memp = reinterpret_cast<unsigned char *>(obj);
        std::reverse(memp, memp + sizeof(T));
    }

} // util namespace

#endif
