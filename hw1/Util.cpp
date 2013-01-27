#include <cmath>
#include <limits>
#include "Util.hpp"

// Floating point comparison functions
bool util::floatEq(GLfloat a, GLfloat b)
{
    return std::fabs(a-b) < std::numeric_limits<GLfloat>::epsilon();
}

bool util::floatLt(GLfloat a, GLfloat b)
{
    return (b-a) > std::numeric_limits<GLfloat>::epsilon();
}

bool util::floatGt(GLfloat a, GLfloat b)
{
    return (a-b) > std::numeric_limits<GLfloat>::epsilon();
}
