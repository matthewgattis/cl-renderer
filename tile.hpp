#pragma once

#include <iostream>
#include <vector>

#include "serializable.hpp"

class Tile : public Serializable
{
public:
    Tile(const std::vector<float> &pixels, int tile_size);
    Tile(std::istream &is);

    const std::vector<float> &get() const;

private:
    std::ostream& serialize(std::ostream& os) const override;
    std::istream& deserialize(std::istream& is) override;

private:
    std::vector<float> pixels_;
    int tile_size_;
};

