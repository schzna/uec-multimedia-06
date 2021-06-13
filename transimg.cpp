#include "bmp.hpp"
#include <cmath>
#include <dlfcn.h>
#include <fstream>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cout << "the num of args is wrong\n";
        return 1;
    }
    std::ifstream ifs(argv[2], std::ios_base::binary);
    auto dll = dlopen(argv[1], RTLD_LAZY);
    if (ifs.is_open() && dll != nullptr)
    {
        auto trans = (size_t(*)(size_t))dlsym(dll, "trans");
        image img(ifs);
        image b = img;
        for (size_t x = 0; x < img.get_width(); x++)
        {
            for (size_t y = 0; y < img.get_height(); y++)
            {
                b.at(x, y).set_lumi(trans(img.at(x, y).get_lumi()));
            }
        }

        std::ofstream ofs(argv[3], std::ios_base::binary);
        b.write(ofs, 8);
        ofs.close();
        ifs.close();
        dlclose(dll);
    }
    else
    {
        char *errstr;

        errstr = dlerror();
        std::cout << errstr << "\n";
    }
    return 0;
}