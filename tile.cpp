#include "tile.hpp"
#include <ostream>
#include <vector>

Tile::Tile(const std::vector<float> &pixels, int tile_size) :
    pixels_(pixels),
    tile_size_(tile_size)
{
}

Tile::Tile(std::istream &is)
{
    deserialize(is);
}

const std::vector<float> &Tile::get() const
{
    return pixels_;
}

std::ostream& Tile::serialize(std::ostream& os) const
{
    os.write((const char *)&tile_size_, sizeof (int));

    os.write(
        (const char *)pixels_.data(),
        4 * tile_size_ * tile_size_ * sizeof (float));

    return os;
}

std::istream& Tile::deserialize(std::istream& is)
{
    int tile_size = 0;
    is.read((char *)&tile_size, sizeof (int));
    tile_size_ = tile_size;

    pixels_.resize(4 * tile_size * tile_size);
    is.read(
        (char *)pixels_.data(),
        4 * tile_size * tile_size * sizeof (float));

    return is;
}

