
#include <chrono>
#include "RNG.hpp"

RNG::RNG(float low, float high)
    : _mt(_rd()),
      _uniformIntDist((int)low, (int)high),
      _uniformFloatDist(low, high)
{
}

int RNG::genUniformInt()
{
    return _uniformIntDist(_mt);
}

float RNG::genUniformFloat()
{
    return _uniformFloatDist(_mt);
}

float RNG::genNormalizedFloat()
{
    static RNG rng(0.0, 1.0);
    return rng.genUniformFloat(); 
}

