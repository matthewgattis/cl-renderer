#include "imagemap.hpp"

ImageMap::ImageMap(int width, int height, int tile_size) :
    width_(width),
    height_(height),
    tile_size_(tile_size)
{
}

ImageMap::ImageMap(const Image& other, int tile_size) :
    width_(other.width()),
    height_(other.height()),
    tile_size_(tile_size)
{
    // Implementation
}

int ImageMap::tile_size() const
{
    return tile_size_;
}

int ImageMap::tile_x_count() const
{
    return width_ / tile_size_ + ((width_ % tile_size_) > 0 ? 1 : 0;
}

int ImageMap::tile_y_count() const
{
    return height_ / tile_size_ + ((height_ % tile_size_) > 0 ? 1 : 0;
}

int ImageMap::width() const
{
    return width_;
}

int ImageMap::height() const
{
    return height_;
}

std::vector<float> ImageMap::pixels() const
{
    std::vector<float> pixels;
    pixels.resize(4 * width * height);
    return pixels;
}

