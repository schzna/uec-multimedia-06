#include <array>
#include <dlfcn.h>
#include <fstream>

int main(int argc, char **argv)
{
    auto dll = dlopen(argv[1], RTLD_LAZY);
    if (dll != nullptr)
    {
        auto trans = (size_t(*)(size_t))dlsym(dll, "trans");
        std::ofstream ofs(argv[2]);
        for (size_t i = 0; i < 256; i++)
        {
            ofs << i << " " << (*trans)(i) << "\n";
        }
    }
}