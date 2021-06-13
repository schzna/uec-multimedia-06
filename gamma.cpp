#include <cmath>
#include <cstddef>

const double gamma_val = 3.0;

extern "C" size_t trans(size_t x)
{
    return static_cast<size_t>(255.0 * std::pow(static_cast<double>(x) / 255.0, 1. / gamma_val));
}