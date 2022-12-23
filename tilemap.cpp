#include "tilemap.hpp"

#include <fstream>

TileMap::TileMap(const std::string &filename)
{
    std::ifstream ifs(filename);
    ifs >> root_;

    int width = getWidth();
    int height = getHeight();
    int tile_size = getTileSize();

    int tile_x_count =
        width / tile_size + ((width % tile_size) > 0 ? 1 : 0);
    int tile_y_count =
        height / tile_size + ((height % tile_size) > 0 ? 1 : 0);

    tile_map_.resize(tile_x_count * tile_y_count);
}

int TileMap::getWidth() const
{
    return root_["width"].asInt();
}

int TileMap::getHeight() const
{
    return root_["height"].asInt();
}

int TileMap::getTileSize() const
{
    return root_["tile_size"].asInt();
}

