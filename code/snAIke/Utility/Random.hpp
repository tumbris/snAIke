#pragma once

#include <random>
#include <type_traits>

template <class T>
T RandInRange(T lower, T upper)
{
    static std::random_device re;
    static std::mt19937_64 mt(re());

    if constexpr (std::is_integral_v<T>)
    {
        std::uniform_int_distribution<T> uid(lower, upper);
        return uid(mt);
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        std::uniform_real_distribution<T> urd(lower, upper);
        return urd(mt);
    }
    else
    {
        static_assert(false, "Cannot generate random number: T must be integer or floationg point type!");
    }
}