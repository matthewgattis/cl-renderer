#include "pngimagewriter.hpp"

#include <stdlib.h>
#include <stdio.h>

#include <png.h>

#define LOG_MODULE_NAME ("PNGImageWriter")
#include "log.hpp"

void _pngcleanup(
    void *fp,
    void *png_ptr,
    void *info_ptr,
    png_bytep row)
{
    if (fp != nullptr)
    {
        fclose((FILE *)fp);
    }
    if (info_ptr != nullptr)
    {
        png_free_data((png_structp)png_ptr, (png_infop)info_ptr, PNG_FREE_ALL, -1);
    }
    if (png_ptr != nullptr)
    {
        png_destroy_write_struct((png_structp *)&png_ptr, (png_infopp)nullptr);
    }
    if (row != NULL)
    {
        delete[] row;
    }
}

void PNGImageWriter::writeImage(
    const std::string &filename,
    const std::vector<float> &pixels,
    int width)
{
    FILE *fp = nullptr;
    fp = fopen(filename.c_str(), "wb");
    if (fp == nullptr)
    {
        LOG_WARNING << "Could not open file (" << filename << ") for writing." << std::endl;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == nullptr)
    {
        LOG_WARNING << "Could not allocate PNG writing structure." << std::endl;
        _pngcleanup(fp, nullptr, nullptr, nullptr);
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr)
    {
        LOG_WARNING << "Could not allocate PNG info struct." << std::endl;
        _pngcleanup(fp, png_ptr, nullptr, nullptr);
    }

    png_bytep row;
    
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        LOG_WARNING << "Error during PNG write operations." << std::endl;
        _pngcleanup(fp, png_ptr, info_ptr, row);
    }

    png_init_io(png_ptr, fp);

    // PNG compression level.
    png_set_compression_level(png_ptr, 9);

    int height = pixels.size() / 3 / width;

    png_set_IHDR(
        png_ptr,
        info_ptr,
        width,
        height,
        16,
        PNG_COLOR_TYPE_RGB,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE,
        PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    png_set_swap(png_ptr);

    row = new png_byte[2 * 3 * width];

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int i = 0; i < 3; i++)
            {
                unsigned short c = 
                    65535.0f * std::max(0.0f, std::min(pixels[3 * (x + y * width) + i], 1.0f));
                ((unsigned short *)row)[3 * x + i] = c;
            }
        }
        png_write_row(png_ptr, row);
    }

    png_write_end(png_ptr, nullptr);
    _pngcleanup(fp, png_ptr, info_ptr, row);
}

