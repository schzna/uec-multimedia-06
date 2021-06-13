#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

enum class compression_type
{
    BI_RGB,
    BI_RLE8,
    BI_RLE4
};

struct __attribute__((packed)) bmp_header
{
    uint8_t id[2] = {0x42, 0x4d};
    uint32_t size_file;
    uint8_t reserved[4] = {0x00, 0x00, 0x00, 0x00};
    uint32_t offset_data;
};

struct __attribute__((packed)) bmp_infoheader
{
    uint32_t size_header = 40;
    uint32_t width;
    uint32_t height;
    uint16_t planes = 1;
    uint16_t bitcount;
    uint32_t compression;
    uint32_t size_image;
    uint32_t res_hor;
    uint32_t res_ver;
    uint32_t colorindex = 256;
    uint32_t important = 0;
};

struct colortable
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t reserved = 0;
};

struct color
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;

    color() = default;
    color(const color &) = default;
    color(color &&) = default;

    color(uint8_t lumi) : blue(lumi), green(lumi), red(lumi)
    {
    }

    uint8_t get_lumi()
    {
        return (blue + red + green) / 3;
    }

    void set_lumi(uint8_t lumi)
    {
        blue = lumi;
        green = lumi;
        red = lumi;
    }
};

class image
{
private:
    std::vector<color> pixels;
    size_t width, height;

    bmp_header header;
    bmp_infoheader infoheader;

public:
    std::vector<colortable> tables;
    image(size_t width, size_t height, int bit = 24)
    {
        pixels = std::vector<color>(width * height, color(255));
        infoheader.bitcount = bit;
        infoheader.compression = static_cast<uint32_t>(compression_type::BI_RGB);
    }

    image(const image &) = default;
    image(image &&) = default;

    size_t get_width()
    {
        return width;
    }

    size_t get_height()
    {
        return height;
    }

    //8bit only
    template <typename T>
    image(std::basic_istream<T> &is)
    {
        is.read(reinterpret_cast<char *>(std::addressof(header)), sizeof(header));
        is.read(reinterpret_cast<char *>(std::addressof(infoheader)), sizeof(infoheader));
        if (infoheader.bitcount <= 8)
        {
            for (size_t i = 0; i < (size_t)std::pow(2, infoheader.bitcount); i++)
            {
                colortable tmp;
                is.read(reinterpret_cast<char *>(std::addressof(tmp)), sizeof(tmp));
                tables.push_back(tmp);
            }
        }
        width = infoheader.width;
        height = infoheader.height;

        int pad = (4 - width % 4) % 4;

        if (infoheader.bitcount == 8)
        {
            for (size_t y = 0; y < height; y++)
            {
                for (size_t x = 0; x < width; x++)
                {
                    uint8_t lumi;
                    is.read(reinterpret_cast<char *>(std::addressof(lumi)), 1);
                    pixels.emplace_back(lumi);
                }
                is.seekg(pad, std::ios_base::cur);
            }
        }
    }

    color &at(size_t x, size_t y)
    {
        return pixels.at(y * width + x);
    }

    //8bit only
    template <typename T>
    void write(std::basic_ostream<T> &os, int bit = 24)
    {
        int pad = (4 - width % 4) % 4;
        long long pl = 0;

        header.offset_data = 0x36;
        header.size_file = 0x36 + bit * (width + pad) * height / 8;

        infoheader.width = static_cast<uint32_t>(width);
        infoheader.height = static_cast<uint32_t>(height);
        infoheader.res_hor = 0;
        infoheader.res_ver = 0;
        infoheader.compression = static_cast<uint32_t>(compression_type::BI_RGB);
        infoheader.bitcount = bit;

        os.write(reinterpret_cast<char *>(std::addressof(header)), sizeof(header));
        os.write(reinterpret_cast<char *>(std::addressof(infoheader)), sizeof(infoheader));

        if (infoheader.bitcount <= 8)
        {
            for (size_t i = 0; i < (size_t)std::pow(2, infoheader.bitcount); i++)
            {
                os.write(reinterpret_cast<char *>(std::addressof(tables.at(i))), sizeof(tables.at(i)));
            }
        }

        if (infoheader.bitcount == 8)
        {
            for (size_t y = 0; y < height; y++)
            {
                for (size_t x = 0; x < width; x++)
                {
                    color &pix = pixels.at(y * width + x);
                    uint8_t lumi = pix.get_lumi();
                    os.write(reinterpret_cast<char *>(std::addressof(lumi)), 1);
                }
                os.write(reinterpret_cast<char *>(std::addressof(pl)), pad);
            }
        }
    }
};