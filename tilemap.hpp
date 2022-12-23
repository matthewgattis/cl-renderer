#pragma once

#include <memory>
#include <vector>

#include <json/json.h>

#include "serializable.hpp"

class Tile;

class TileMap
{
public:
    TileMap(const std::string &filename);

    int getWidth() const;
    int getHeight() const;
    int getTileSize() const;

private:
    std::vector<std::shared_ptr<Tile>> tile_map_;
    Json::Value root_;
};

