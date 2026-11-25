#pragma once
#include <random>

namespace RandomUtils
{
    inline std::mt19937& Engine()
    {
        static std::mt19937 engine(std::random_device{}());
        return engine;
    }

    inline float Float(float min, float max)
    {
        return std::uniform_real_distribution<float>(min, max)(Engine());
    }

    inline int Int(int min, int max)
    {
        return std::uniform_int_distribution<int>(min, max)(Engine());
    }
}
