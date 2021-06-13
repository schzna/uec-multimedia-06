#include <cmath>
#include <cstddef>
#include <utility>

const double range = 10.;
const double avg = 5.;
const double var = 0.01;

const double pi = 3.1415926535;

extern "C" size_t trans(size_t i)
{
    double x = static_cast<double>(i) * range / 255.;
    double y = static_cast<size_t>(255. * std::exp(-(x - avg) * (x - avg) / (2. * var * var)) / (std::sqrt(2 * pi) * var));
    return std::max(std::min(static_cast<size_t>(y), (size_t)255), (size_t)0);
}