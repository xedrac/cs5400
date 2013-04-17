#ifndef RNG_HPP
#define RNG_HPP

#include <random>


class RNG
{
public:
    RNG(float low, float high);

    int   genUniformInt();
    float genUniformFloat();

private:
    std::random_device _rd;
    std::mt19937 _mt;
    std::uniform_int_distribution<int>    _uniformIntDist;
    std::uniform_real_distribution<float> _uniformFloatDist;
};


#endif
