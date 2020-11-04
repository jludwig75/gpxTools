#pragma once

#include <cmath>

template<typename Value>
Value pathag(Value a, Value b)
{
    return std::sqrt(a * a + b * b);
}

