#include "bmp.hpp"
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "the num of args is wrong\n";
        return 1;
    }
    std::array<size_t, 256> data;
    data.fill(0);
    std::ifstream ifs(argv[1], std::ios_base::binary);
    if (ifs.is_open())
    {
        image img(ifs);
        for (size_t x = 0; x < img.get_width(); x++)
        {
            for (size_t y = 0; y < img.get_height(); y++)
            {
                data[img.at(x, y).get_lumi()]++;
            }
        }
    }
    std::ofstream ofs(argv[2]);
    for (size_t i = 0; i < 256; i++)
    {
        ofs << i << " " << data[i] << "\n";
    }
    return 0;
}